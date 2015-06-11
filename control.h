#ifndef CONTROL_H
#define CONTROL_H

#include <vector>
#include "hopper.h"
#include "map.h"

typedef std::vector< double > state_type;
enum Phase {STANCE, BALLISTIC, INIT};

class Control
{
public:
    enum Axis {XAXIS, YAXIS, ZAXIS};

    Control();
    Control(double timeStep, bool mainSimFlag);
    ~Control();

    /* Get hopper model */
    Hopper& getHopper();
    /* Get phase */
    Phase getPhase();
    /* Compute positions, velocities and accelerations in ballistic phase. */
    void hopperBallisticTrajectory();
    /* Compute length and angles and their velocities and accelerations in stance phase. */
    void hopperStancePhase();
    /* Describes differential equations during stance phase */
    void operator()( const state_type &x , state_type &dxdt , const double t );
    /* Rotate hopper by some angle alfa and recompute down position coordinates. */
    void rotateHopper(double alfa, double beta);
    /* Make transition to stance phase. Recompute new polar coordinates values. */
    void transitionToStancePhase();
    /* Update down position of the hopper (due to change of the alfa angle). */
    void updateDownPos();

    constexpr static double gravityConst = 9.81;
    double timeStepSec;

private:
    /* Conversion from cartesian coordinates to polar. Axis must be specified. */
    double cartesianToPolar(Axis axis);
    /* Get stance parameters. */
    state_type getStanceParams();
    /* Set new stance phase parameters after numerical integration step was performed. */
    void setStanceParams(state_type x, double countNum);
    /* Make transition to ballistic phase. Recompute new velocities. */
    void transitionToBallisticPhase();
    /* Update position of hopper in flight after new velocities were apllied. */
    void updatePos();

    bool mainSimFlag;
    /* Model of the hopper. */
    Hopper hopper;
    /* Current phase of movement. Can be ballistic or stance. */
    Phase phase;
    Map map;
};

#endif // CONTROL_H
