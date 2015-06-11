#include "simulation.h"
#include "glwidget.h" // for HopperPosition type
#include "math.h"

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

#include <functional>
#include <boost/numeric/odeint.hpp>

Simulation::Simulation(double timeStep) : timeStepSec(timeStep),
                                          control(timeStep, true)
{
//    control.rotateHopper(0.8, 0.0);
}

Simulation::~Simulation()
{
}

void Simulation::countEnergy()
{
    double vSum = sqrt(pow(sqrt(
                          pow(this->control.getHopper().getVx(),2) +
                          pow(this->control.getHopper().getVy(),2)),2)
                     + pow(this->control.getHopper().getVz(),2));

    double ekTemp = ((Hopper::mConst * pow(vSum,2)) / 2.0);

    double ephTemp = Hopper::mConst * Control::gravityConst * this->control.getHopper().getY();

    double epsTemp = (Hopper::kConst * pow(Hopper::maxLength - this->control.getHopper().getLength(),2)) / 2.0;

    this->control.getHopper().setEk(ekTemp);
    this->control.getHopper().setEph(ephTemp);
    this->control.getHopper().setEps(epsTemp);

    double energy = ekTemp + ephTemp + epsTemp;
    double kk = 0.0;
    double kkk = kk;
}

Control& Simulation::getControl()
{
    return this->control;
}

HopperData Simulation::step()
{
    switch(control.getPhase())
    {
        case BALLISTIC:
            control.hopperBallisticTrajectory();
            break;
        case STANCE:
            control.hopperStancePhase();
            break;
    }

    this->countEnergy();
    HopperData hopperDataTemp{this->control.getHopper().getXDown(),
                              this->control.getHopper().getYDown(),
                              this->control.getHopper().getZDown(),
                              this->control.getHopper().getAlfa(),
                              this->control.getHopper().getBeta(),
                              this->control.getHopper().getLength()};

    return hopperDataTemp;
}
