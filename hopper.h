#ifndef HOPPER_H
#define HOPPER_H


#include <QMetaType>
struct BackgroundSimData
{
    BackgroundSimData() : startingX(0.0),
                          startingY(0.0),
                          startingZ(0.0),
                          startingAlfa(0.0),
                          startingBeta(0.0),
                          startingVx(0.0),
                          startingVy(0.0),
                          startingVz(0.0),
                          targetX(0.0),
                          targetZ(0.0),
                          initPhaseFlag(0.0)
    {}

    BackgroundSimData(double nstartingX,
                      double nstartingY,
                      double nstartingZ,
                      double nstartingAlfa,
                      double nstartingBeta,
                      double nstartingVx,
                      double nstartingVy,
                      double nstartingVz,
                      double ntargetX,
                      double ntargetZ,
                      bool ninitPhaseFlag)
                    : startingX(nstartingX),
                      startingY(nstartingY),
                      startingZ(nstartingZ),
                      startingAlfa(nstartingAlfa),
                      startingBeta(nstartingBeta),
                      startingVx(nstartingVx),
                      startingVy(nstartingVy),
                      startingVz(nstartingVz),
                      targetX(ntargetX),
                      targetZ(ntargetZ),
                      initPhaseFlag(ninitPhaseFlag)
    {}

    BackgroundSimData(const BackgroundSimData &other)
    {
        startingX  = other.startingX;
        startingY  = other.startingY;
        startingZ  = other.startingZ;
        startingAlfa  = other.startingAlfa;
        startingBeta  = other.startingBeta;
        startingVx  = other.startingVx;
        startingVy  = other.startingVy;
        startingVz  = other.startingVz;
        targetX  = other.targetX;
        targetZ  = other.targetZ;
        initPhaseFlag = other.initPhaseFlag;
    }

    ~BackgroundSimData() { }

    double startingX;
    double startingY;
    double startingZ;

    double startingAlfa;
    double startingBeta;

    double startingVx;
    double startingVy;
    double startingVz;

    double targetX;
    double targetZ;

    bool initPhaseFlag;
};

Q_DECLARE_METATYPE(BackgroundSimData)


class Hopper
{
public:
    Hopper();
    Hopper(double length);

    void resetAllParams();
    void setNewParams(BackgroundSimData backgroundSimData);

    /* Setters */
    void setX(double newX);
    void setY(double newY);
    void setZ(double newZ);
    void setXDown(double newXDown);
    void setYDown(double newYDown);
    void setZDown(double newZDown);
    void setVx(double newVx);
    void setVy(double newVy);
    void setVz(double newVz);
    void setAlfa(double newAlfa);
    void setLength(double newLength);
    void setBeta(double newBeta);
    void setVLength(double newVLength);
    void setVAlfa(double newVAlfa);
    void setVBeta(double newVBeta);
    void setALength(double newALength);
    void setAAlfa(double newAAlfa);
    void setABeta(double newABeta);
    void setEk(double newEk);
    void setEph(double newEph);
    void setEps(double newEps);

    /* Getters */
    double getX();
    double getY();
    double getZ();
    double getXDown();
    double getYDown();
    double getZDown();
    double getAlfa();
    double getBeta();
    double getVy();
    double getVx();
    double getVz();
    double getLength();
    double getVLength();
    double getVAlfa();
    double getVBeta();
    double getALength();
    double getAAlfa();
    double getABeta();
    double getEk();
    double getEph();
    double getEps();

    constexpr static double kConst = 106.0;
    constexpr static double mConst = 1.0;
    constexpr static double maxLength = 1.0;

private:
    double length;

    double x, y, z;

    double xDown, yDown, zDown;

    double alfa, beta;

    double vx, vy, vz;

    double vLength, vAlfa, vBeta;

    double aLength, aAlfa, aBeta; // DO WYRZUCENIA

    double ek, eph, eps;
};


#endif // HOPPER_H
