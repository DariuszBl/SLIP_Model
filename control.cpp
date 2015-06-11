#include "control.h"
#include "glwidget.h" // for HopperPosition type

#include "math.h"

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

#include <functional>
#include <boost/numeric/odeint.hpp>


Control::Control() : timeStepSec(0.001),
                     phase(BALLISTIC),
                     hopper(),
                     map(0, 0, 0, 0)
{
    map.load("trasa4.dat");
}


Control::Control(double timeStep, bool mainSimFlag) : timeStepSec(timeStep),
                                                      phase(BALLISTIC),
                                                      hopper(),
                                                      mainSimFlag(mainSimFlag),
                                                      map(0, 0, 0, 0)
{
//    rotateHopper(0.0);

    map.load("trasa4.dat");
    //   this->hopper.setBeta(atan2(this->hopper.getVz(),this->hopper.getVx()) * 180.0 / M_PI);
}

Control::~Control()
{
}

double Control::cartesianToPolar(Axis axis)
{
    switch(axis)
    {
        case XAXIS:
        {
            double tempX = hopper.getLength() * sin( hopper.getAlfa() * M_PI / 180.0)*
                            cos(hopper.getBeta() * M_PI / 180.0);
            //error
            if(fabs(tempX) < 0.0000001)
                return 0.0;
            else
                return tempX;
            break;
        }
        break;
        case YAXIS:
        {
            double tempY = (this->hopper.getLength() * cos(hopper.getAlfa() * M_PI / 180.0));
            //error
            if(fabs(tempY) < 0.0000001)
                return 0.0;
            else
                return tempY;
            break;
        }
        break;
        case ZAXIS:
        {
            double tempZ =  (this->hopper.getLength() * sin(hopper.getAlfa() * M_PI / 180.0)*
                             sin(this->hopper.getBeta() * M_PI / 180.0));
            //error
            if(fabs(tempZ) < 0.0000001)
                return 0.0;
            else
                return tempZ;
            break;
        }
        break;
    }

    /* defensive coding */
    return 0.0;
}

Hopper& Control::getHopper()
{
    return this->hopper;
}

Phase Control::getPhase()
{
    return this->phase;
}

state_type Control::getStanceParams()
{
    state_type temp = {this->hopper.getLength(),
                       this->hopper.getVLength(),
                       this->hopper.getAlfa() * M_PI / 180.0,
                       this->hopper.getVAlfa(),
                       this->hopper.getBeta() * M_PI / 180.0,
                       this->hopper.getVBeta()};
    return temp;
}

void Control::hopperBallisticTrajectory()
{
    updatePos();

    // Update velocity in y axis
    double deltaVy = -gravityConst*timeStepSec;
    this->hopper.setVy(this->hopper.getVy() + deltaVy);

    //Wait for hopper to take off
    static int counter = 0;

    // When hopper touches the ground after flight
    if ((this->hopper.getYDown() <= 0.0) && (counter == 0))
    {
        transitionToStancePhase();
        counter = 5;
    }

    if (counter != 0)
        counter--;
}

void Control::hopperStancePhase()
{
    static int counter = 0;

    int countNum;
    if(mainSimFlag)
        countNum = 100;
    else
        countNum = 1;

    while(counter < countNum)
    {
        // get stance parameters
        state_type x = getStanceParams();

        // step numerical integration
        boost::numeric::odeint::runge_kutta4< state_type > rk4;
        rk4.do_step( std::ref(*this), x, 0.0, timeStepSec / double(countNum));

        // set stance parameters
        setStanceParams(x, countNum);

        if (this->hopper.getLength() >= Hopper::maxLength)
        {
            transitionToBallisticPhase();
            break;
        }
        counter++;
    }
    counter = 0;
}

/*
r = x[0]
r' = x[1]

alfa = x[2]
alfa' = x[3]

beta = x[4]
beta' = x[5]
*/
void Control::operator()( const state_type &x , state_type &dxdt , const double t )
{
    dxdt[0] = x[1];
    dxdt[1] = (x[0] * pow(x[3],2)) +
              (x[0] * pow(x[5],2) * pow(sin(x[2]),2)) -
              ((Hopper::kConst/Hopper::mConst) * (x[0] - Hopper::maxLength)) -
              (Control::gravityConst * cos(x[2]));

    dxdt[2] = x[3];
    dxdt[3] = (pow(x[5],2) * sin(x[2]) * cos(x[2])) +
              ((Control::gravityConst * sin(x[2])) / x[0]) -
              ((2.0 * x[1] * x[3]) / x[0]);

    dxdt[4] = x[5];

    if (x[2] == 0.0)
    {
        dxdt[5] = 0.0;
    }
    else
    {
        dxdt[5] = -((2.0 * x[5] * x[3] * cos(x[2])) / sin(x[2])) -
                   ((2.0 * x[1] * x[5]) / x[0]);
    }
}

void Control::rotateHopper(double alfa, double beta)
{
    this->hopper.setAlfa(alfa);
    this->hopper.setBeta(beta);
    updateDownPos();
}

void Control::setStanceParams(state_type x, double countNum)
{
    this->hopper.setLength(x[0]);
    this->hopper.setVLength(x[1]);

    // error
    if (fabs(x[2]) < 0.0000001)
        this->hopper.setAlfa(0.0);
    else
        this->hopper.setAlfa(x[2] / M_PI * 180.0);

    // error
    if (fabs(x[3]) < 0.0000001)
        this->hopper.setVAlfa(0.0);
    else
        this->hopper.setVAlfa(x[3]);

    // error
    if (fabs(x[4]) < 0.0000001)
        this->hopper.setBeta(0.0);
    else
        this->hopper.setBeta(x[4] / M_PI * 180.0);

    // error
    if (fabs(x[5]) < 0.0000001)
        this->hopper.setVBeta(0.0);
    else
        this->hopper.setVBeta(x[5]);

    // Update position
    double newX = this->hopper.getXDown() + cartesianToPolar(XAXIS);
    double newY = this->hopper.getYDown() + cartesianToPolar(YAXIS);
    double newZ = this->hopper.getZDown() + cartesianToPolar(ZAXIS);

    // Update velocities
    double velX = ((newX - this->hopper.getX()) / (this->timeStepSec)) * countNum; /// countNum);
    double velY = ((newY - this->hopper.getY()) / (this->timeStepSec))* countNum; /// countNum);
    double velZ = ((newZ - this->hopper.getZ()) / (this->timeStepSec))* countNum; /// countNum);

    this->hopper.setVx(velX);
    this->hopper.setVy(velY);
    this->hopper.setVz(velZ);

    this->hopper.setX(newX);
    this->hopper.setY(newY);
    this->hopper.setZ(newZ);
}

void Control::transitionToBallisticPhase()
{
    this->phase = BALLISTIC;
    this->hopper.setLength(Hopper::maxLength);
    this->hopper.setY(this->hopper.getYDown() + cartesianToPolar(YAXIS));

    double alfaRad = this->hopper.getAlfa() * M_PI / 180.0;
    double betaRad = this->hopper.getBeta() * M_PI / 180.0;

    double vxTemp = this->hopper.getVLength() * sin(alfaRad) * cos(betaRad) +
                    this->hopper.getVAlfa() * this->hopper.getLength() * cos(alfaRad) * cos(betaRad) -
                    this->hopper.getVBeta() * this->hopper.getLength() * sin(alfaRad) * sin(betaRad);

    double vzTemp = this->hopper.getVLength() * sin(alfaRad) * sin(betaRad) +
                    this->hopper.getLength() * (this->hopper.getVAlfa() * cos(alfaRad) * sin(betaRad) +
                    this->hopper.getVBeta() * sin(alfaRad) * cos(betaRad));

    double vyTemp = (this->hopper.getVLength() * cos(alfaRad)) -
                    (this->hopper.getLength() * this->hopper.getVAlfa() * sin(alfaRad));

    this->hopper.setVx(vxTemp);
    this->hopper.setVy(vyTemp);
    this->hopper.setVz(vzTemp);

    this->hopper.setVLength(0.0);
    this->hopper.setVBeta(0.0);
    this->hopper.setVAlfa(0.0);
}

void Control::transitionToStancePhase()
{
    // Change current phase (ballistic or stance)
    this->phase = STANCE;
    // Count length for every hopper axis
    double xLength = hopper.getX() - hopper.getXDown();
    double zLength = hopper.getZ() - hopper.getZDown();

    this->hopper.setYDown(0.0);
    this->hopper.setY(this->hopper.getYDown() + cartesianToPolar(YAXIS));

    // Count hopper length derivative
    double vLengthTemp = 1.0 / sqrt(pow(xLength,2) + pow(zLength,2) + pow(this->hopper.getY(),2)) *
                               ((xLength * this->hopper.getVx()) +
                                (zLength * this->hopper.getVz()) +
                                (this->hopper.getY() * this->hopper.getVy()));

    // Count beta angle derivative
    double vBetaTemp;
    if(xLength == 0.0)
    {
        vBetaTemp = 0.0;
    }
    else
    {
        vBetaTemp = ((this->hopper.getVz() * xLength - this->hopper.getVx() * zLength) /
                        pow(xLength,2))/
                        (1.0 + pow(zLength / xLength, 2));
    }

    double vAlfaTemp;
    if(xLength == 0.0 && zLength == 0.0)
    {
        vAlfaTemp = 0.0;
    }
    else
    {
        // Count alfa angle derivative
        vAlfaTemp = (hopper.getY() * (xLength * hopper.getVx() + zLength * hopper.getVz()) -
                     (hopper.getVy() * (pow(xLength,2) + pow(zLength,2)))) / sqrt(pow(xLength,2) +
                     pow(zLength,2)) / (pow(xLength,2) + pow(zLength,2) + pow(hopper.getY(),2));
    }

    this->hopper.setVLength(vLengthTemp);
    this->hopper.setVBeta(vBetaTemp);
    this->hopper.setVAlfa(vAlfaTemp);
}

void Control::updatePos()
{
    // Update velocity and position in Y axis
    double deltaPosY = (this->hopper.getVy()*this->timeStepSec) - (gravityConst * pow(timeStepSec,2.0)) / 2.0;
    this->hopper.setY(this->hopper.getY() + deltaPosY);
    this->hopper.setYDown(this->hopper.getYDown() + deltaPosY);

    // Position change in X axis
    double deltaPosX = this->hopper.getVx() * timeStepSec;
    this->hopper.setX(this->hopper.getX() + deltaPosX);
    this->hopper.setXDown(this->hopper.getXDown() + deltaPosX);

    // Position change in Z axis
    double deltaPosZ = this->hopper.getVz() * timeStepSec;
    this->hopper.setZ(this->hopper.getZ() + deltaPosZ);
    this->hopper.setZDown(this->hopper.getZDown() + deltaPosZ);
}

void Control::updateDownPos()
{
    double newXDown = this->hopper.getX() - cartesianToPolar(XAXIS);
    double newYDown = this->hopper.getY() - cartesianToPolar(YAXIS);
    double newZDown = this->hopper.getZ() - cartesianToPolar(ZAXIS);

    this->hopper.setXDown(newXDown);
    this->hopper.setYDown(newYDown);
    this->hopper.setZDown(newZDown);
}
