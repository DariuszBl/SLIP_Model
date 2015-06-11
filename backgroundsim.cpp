#include "backgroundsim.h"

#include <QApplication>
#include <QThread>
#include <QTimer>
#include <QEventLoop>
#include <windows.h>
#include <ctime>
#include "float.h"

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

BackgroundSim::BackgroundSim(QObject *parent) :
    QObject(parent),
    simData{},
    control(0.01, false) // timestep equal to 0.1
{
    anglesFound = true;
    working = false;
    abort   = false;
}

void BackgroundSim::requestWork()
{
    mutex.lock();
    working = true;
    abort = false;
    mutex.unlock();

    emit workRequested();
}

void BackgroundSim::abortFun()
{
    mutex.lock();
    if (working)
    {
        abort = true;
    }
    mutex.unlock();
}

void BackgroundSim::doWork()
{
    while(1)
    {
        mutex.lock();
        if (!abort) {
            condition.wait(&mutex);
        }
        else
        {
            emit finished();
            return;
        }
        mutex.unlock();

        findAngles();
    }
}

void BackgroundSim::findAngles()
{
    /* Init phase */
//    srand(time(NULL));
    double optInitBeta; // = rand() % 360;
    double optInitAlfa;

    double distance;
    double distanceOld = DBL_MAX;

    double alfaInc = 0.5;
    double betaInc = 30.0;

    bool distanceIsDecreasing = true;

    /* Find optimal starting beta */

    double distanceInitX = fabs(simData.startingX - simData.targetX);
    double distanceInitZ = fabs(simData.startingZ - simData.targetZ);

    /* Check which quadrant it is */
    /* I quadrant */
    if(simData.startingX >= simData.targetX && simData.startingZ <= simData.targetZ)
    {
        optInitBeta = (atan2(distanceInitZ,distanceInitX) * 180.0 / M_PI) + 90.0;
    }
    /* II quadrant */
    else if(simData.startingX <= simData.targetX && simData.startingZ <= simData.targetZ)
    {
        optInitBeta = atan2(distanceInitZ,distanceInitX);
    }
    /* III quadrant */
    else if(simData.startingX <= simData.targetX && simData.startingZ >= simData.targetZ)
    {
        optInitBeta = (atan2(distanceInitZ,distanceInitX) * 180.0 / M_PI) + 270.0;
    }
    /* IV quadrant */
    else
    {
        optInitBeta = (atan2(distanceInitZ,distanceInitX) * 180.0 / M_PI) + 180.0;
    }

    double initAlfa = 0.01;
    /* Optimal init beta was found. Now increase alfa and check if distance decreases.
     * Stop when distance has increased instead. */
    while (distanceIsDecreasing)
    {
        distance = simToTarget(initAlfa, optInitBeta);

        if(distance >= distanceOld)
        {
            distanceIsDecreasing = false;
            initAlfa -= alfaInc;
            /* if first alfa is bad back to initVal */
            if(initAlfa < 0.0)
                initAlfa = 0.01;
            optInitAlfa = initAlfa;
            distance = distanceOld;
        }
        else
        {
            distanceOld = distance;
            initAlfa += alfaInc;
        }
    }

    /* Check if we need to reduce speed */
    if (initAlfa == 0.01)
    {
        distance = simToTarget(initAlfa, optInitBeta);

        if(distance >= distanceOld)
        {
            /* break */
            optInitAlfa = 0;
        }
    }

    /* After optimal init alfa and beta were found algorithm starts adjusting those parameters until
     * model reaches expected point with preset accuracy. */

    double workingBeta = optInitBeta;
    double workingAlfa = optInitAlfa;

    distanceOld = simToTarget(workingAlfa, workingBeta);

    bool adjustBeta = true;
    bool adjustAlfa = false;

    bool directionRight = true;

    distanceIsDecreasing = true;

    while (!anglesFound)
    {
        static int adjustCounter = 0;

        adjustCounter++;

        /* Find beta */

        if(adjustBeta)
        {
            if(directionRight)
            {
                workingBeta += betaInc;
            }
            else
            {
                workingBeta -= betaInc;
            }

            distance = simToTarget(workingAlfa, workingBeta);

            if(distance >= distanceOld)
            {
                /* change direction */
                if (directionRight)
                {
                    directionRight = false;
                    workingBeta -= betaInc;
                    distance = distanceOld;
                }
                else
                {
                    adjustBeta = false;
                    adjustAlfa = true;
                    directionRight = true;
                    workingBeta += betaInc;
                    betaInc /= 2.0;
                    distance = distanceOld;
                }
            }
            else
            {
                distanceOld = distance;
            }
        }

        /* Find alfa */
        if(adjustAlfa)
        {
            if(directionRight)
            {
                workingAlfa += alfaInc;
            }
            else
            {
                workingAlfa -= alfaInc;
            }

            distance = simToTarget(workingAlfa, workingBeta);

            if(distance >= distanceOld)
            {
                /* change direction */
                if (directionRight)
                {
                    directionRight = false;
                    workingAlfa -= alfaInc;
                    distance = distanceOld;
                }
                else
                {
                    adjustBeta = true;
                    adjustAlfa = false;
                    directionRight = true;
                    workingAlfa += alfaInc;
                    alfaInc /= 2.0;
                    distance = distanceOld;
                }
            }
            else
            {
                distanceOld = distance;
            }
        }

        if ((distance <= 0.02) || (adjustCounter > 60))
        {
            anglesFound = true;

            adjustCounter = 0;

            sendAnglesToMain(workingAlfa, workingBeta);

            if (distance <= 0.02)
            {
                targetReached = true;
                emit requestNewTarget();
            }
        }
    }
}

double BackgroundSim::simToTarget(double alfa, double beta)
{
    /* This sim have 3 phases:
       I Phase -> Ballistic
       II Phase -> Stance
       III Phase -> Ballistic again
    */

    /* INIT */
    control.getHopper().resetAllParams();
    control.getHopper().setNewParams(simData);

    control.rotateHopper(alfa, beta);

    /* PHASE I */
    /* Count distance travelled in ballistic phase (PHASE I)*/
    double distanceTravelledX;
    double distanceTravelledZ;
    double timePhase1;

    /* Check if this is initial phase or not*/
    if(simData.initPhaseFlag)
    {
        double distY = simData.startingY - (Hopper::maxLength * cos(alfa*M_PI/180.0));
        timePhase1 = ( simData.startingVy + sqrt(pow(simData.startingVy,2.0) + 2.0 * distY *
                               Control::gravityConst))
                      / Control::gravityConst;

        control.getHopper().setY(simData.startingY - distY);
        control.getHopper().setVy(simData.startingVy - Control::gravityConst * timePhase1);

    }
    else
    {
        /* Count time this phase takes */
        timePhase1 = 2.0 * control.getHopper().getVy() / Control::gravityConst;
        control.getHopper().setVy(-simData.startingVy);
    }


    /* End of PHASE I. New position of hopper, Y is the same is in the start. Preperations for PHASE II. */
    /* Count distance hopper travels in this phase */
    /* Y is the same as in the beginning */
    distanceTravelledX = control.getHopper().getVx() * 2.0 * timePhase1;
    distanceTravelledZ = control.getHopper().getVz() * 2.0 * timePhase1;

    double newPosX = simData.startingX + distanceTravelledX;
    double newPosZ = simData.startingZ + distanceTravelledZ;

    control.getHopper().setX(newPosX);
    control.getHopper().setZ(newPosZ);

    control.updateDownPos();

    /* Start of PHASE II */

    control.transitionToStancePhase();

    /* Run solver unitl transition to STANCE is done */
    while(control.getPhase() == STANCE)
    {
        control.hopperStancePhase();
    }

    /* Start of PHASE III */

    /* Count distance travelled in ballistic phase (PHASE I)*/

    /* Count time this phase takes */
    double timePhase3 = 2.0 * control.getHopper().getVy() / Control::gravityConst;

    /* Count distance hopper travels in this phase */
    distanceTravelledX = control.getHopper().getVx() * timePhase3;
    distanceTravelledZ = control.getHopper().getVz() * timePhase3;

    /* End of PHASE I. New position of hopper, Y is the same is in the start. Preperations for PHASE II. */

    double finalPosX = control.getHopper().getX() + distanceTravelledX;
    double finalPosZ = control.getHopper().getZ() + distanceTravelledZ;

    /* Check if speed is below 0.5 m/s */
    if ((sqrt(pow(control.getHopper().getVx(), 2.0) + pow(control.getHopper().getVz(), 2.0)) > 0.5) ||
        (fabs(control.getHopper().getAlfa()) > 25.0))
    {
        return DBL_MAX;
    }
    else
    {
        /* Return euclidean distance */
        return sqrt(pow(finalPosX - this->simData.targetX, 2.0) +
                    pow(finalPosZ - this->simData.targetZ, 2.0));
    }
}

void BackgroundSim::receiveData(BackgroundSimData simData)
{
   QMutexLocker locker(&mutex);
   this->simData = simData;
   anglesFound = false;
   condition.wakeOne();
}
