#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <Vector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    simulation(0.01),
    ui(new Ui::MainWindow),
    chartsViewer(NULL),
    backgroundDataRequested(true),
    lazyFlag(false)
{
    ui->setupUi(this);

    // Initialize timer
    timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(processing()));
    timer->start(simulation.timeStepSec * 1000.0);

    // The thread and the worker are created in the constructor so it is always safe to delete them.
    thread = new QThread();
    backgroundSim = new BackgroundSim();

    backgroundSim->moveToThread(thread);
    QObject::connect(backgroundSim, SIGNAL(workRequested()), this->thread, SLOT(start()));
    QObject::connect(thread, SIGNAL(started()), this->backgroundSim, SLOT(doWork()));
    QObject::connect(backgroundSim, SIGNAL(finished()), this->thread, SLOT(quit()), Qt::DirectConnection);
    QObject::connect(backgroundSim, SIGNAL(sendAnglesToMain(double, double)), this, SLOT(receiveAngles(double, double)));
    QObject::connect(backgroundSim, SIGNAL(requestNewTarget()), this, SLOT(targetRequested()));

    this->startBackgroundSimThread();
}

MainWindow::~MainWindow()
{
    backgroundSim->abortFun();
    thread->wait();

    delete thread;
    delete backgroundSim;
    delete timer;
    delete ui;
}

ChartsData MainWindow::getCurrentModelData()
{
    return
    {
        /*.ek=*/  this->simulation.getControl().getHopper().getEk(),
        /*.eph=*/ this->simulation.getControl().getHopper().getEph(),
        /*.eps=*/ this->simulation.getControl().getHopper().getEps()
    };
}

void MainWindow::on_Charts_clicked()
{
    this->lazyFlag = true;

    this->chartsViewer = new ChartsViewer(this);
    this->chartsViewer->show();
}

void MainWindow::processing()
{
//    static int processCounter = 0;
//    HopperData newHopperData;

//    if(simulation.getControl().getPhase() == BALLISTIC)
//    {
//        if(processCounter == 300)
//        {
//            simulation.timeStepSec = 0.03;
//            simulation.getControl().timeStepSec = 0.03;
            // Simulation step
    HopperData newHopperData = simulation.step();
//        }
//    }
//    else if(simulation.getControl().getPhase() == STANCE)
//    {
////        simulation.timeStepSec = 0.0001;
////        simulation.getControl().timeStepSec = 0.0001;
//        // Simulation step
//        newHopperData = simulation.step();
//    }

//    if(processCounter == 300)
//    {
            double arrayOfTargets[10] = {3.0, 0.0, 0.0, 0.0, 0.0, 3.0, 3.0, 0.0, 0.0, 3.0};
            static int i = -2;

            if(backgroundDataRequested)
            {
                if (i < 7)
                    i+=2;
                backgroundDataRequested = false;
            }
            sendToBackgroundSim(arrayOfTargets[i], arrayOfTargets[i+1]);

        // Update view
        ui->glWidget->updateHopperData(newHopperData);
        ui->glWidget->update();
        if (this->lazyFlag && (this->chartsDataVector.length() < 4000))
        {
            // Update chart
            static int counter = 0;
            counter++;
            if(counter == 1)
            {
                this->chartsDataVector.append(getCurrentModelData());
                this->chartsViewer->updateChart(&this->chartsDataVector);
                counter = 0;
            }
        }
//        processCounter = 0;
//    }
//    processCounter++;
}

void MainWindow::sendToBackgroundSim(double targetX, double targetZ)
{
    static Phase previousPhase = INIT;
    static Phase currentPhase = INIT;

    previousPhase = currentPhase;
    currentPhase = simulation.getControl().getPhase();

    /* Check if transition to stance was just made or simulation just started. */
    if((previousPhase == STANCE && currentPhase == BALLISTIC) ||
        previousPhase == INIT )
    {
        bool initPhaseFlag;
        (previousPhase == INIT) ? initPhaseFlag = true : initPhaseFlag = false;

        BackgroundSimData backgroundSimDataTemp = BackgroundSimData
        (
            simulation.getControl().getHopper().getX(),
            simulation.getControl().getHopper().getY(),
            simulation.getControl().getHopper().getZ(),
            simulation.getControl().getHopper().getAlfa(),
            simulation.getControl().getHopper().getBeta(),
            simulation.getControl().getHopper().getVx(),
            simulation.getControl().getHopper().getVy(),
            simulation.getControl().getHopper().getVz(),
            targetX,
            targetZ,
            initPhaseFlag
        );
        backgroundSim->receiveData(backgroundSimDataTemp);
    }
}

void MainWindow::startBackgroundSimThread()
{
    // To avoid having two threads running simultaneously, the previous thread is aborted.
    backgroundSim->abortFun();
    thread->wait(); // If the thread is not running, this will immediately return.

    backgroundSim->requestWork();
}

void MainWindow::targetRequested()
{
    backgroundDataRequested = true;
}

void MainWindow::receiveAngles(double alfa, double beta)
{
    static int counter = 0;
    std::vector<double> alfaVector = {0.0, 0.0, 0.0, -10.0, -10.0, 2.0, 0.0, 0.0, 0.0, 0.0, -15.0, 15.0, 0.0, 0.0};
    //    double localAlfa = alfa;

    simulation.getControl().rotateHopper(alfaVector[counter], 0.0);
    counter++;
}
