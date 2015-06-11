#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include "control.h"

struct HopperData;

class Simulation
{

public:
    enum Axis {XAXIS, YAXIS, ZAXIS};
//    enum Phase {STANCE, BALLISTIC};

    Simulation(double timeStep);
    ~Simulation();

    /* Get control instance. */
    Control& getControl();
    /* Performs one sterp of odeint numerical integration in stance phase */
    HopperData step();

    double timeStepSec;

private:
    /* Counts current energy. */
    void countEnergy();

    /* Control class instance which consist of main model. */
    Control control;

};

#endif // SIMULATION_H
