#include "hopper.h"
#include "math.h"

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

Hopper::Hopper() : length(1.0),
                   x(0.0),
                   y(1.2 * this->length),
                   z(0.0),
                   xDown(0.0),
                   yDown(y - this->length),
                   zDown(0.0),
                   alfa(0.0),
                   vx(0.2),
                   vy(0.0),
                   vz(0.0),
                   vLength(0.0),
                   vAlfa(0.0),
                   vBeta(0.0),
                   aLength(0.0),
                   aAlfa(0.0),
                   aBeta(0.0)
{
    this->beta = atan2(vz,vx) * 180.0 / M_PI;
}

void Hopper::resetAllParams()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->xDown = 0.0;
    this->yDown = 0.0;
    this->zDown = 0.0;
    this->alfa = 0.0;
    this->vx = 0.0;
    this->vy = 0.0;
    this->vz = 0.0;
    this->vLength = 0.0;
    this->vAlfa = 0.0;
    this->vBeta = 0.0;
    this->aLength = 0.0;
    this->aAlfa = 0.0;
    this->aBeta = 0.0;
}

void Hopper::setNewParams(BackgroundSimData backgroundSimData)
{
    this->x = backgroundSimData.startingX;
    this->y = backgroundSimData.startingY;
    this->z = backgroundSimData.startingZ;
    this->vx = backgroundSimData.startingVx;
    this->vy = backgroundSimData.startingVy;
    this->vz = backgroundSimData.startingVz;
}

void Hopper::setX(double newX)
{
    this->x = newX;
}

void Hopper::setY(double newY)
{
    this->y = newY;
}

void Hopper::setZ(double newZ)
{
    this->z = newZ;
}

void Hopper::setXDown(double newXDown)
{
    this->xDown = newXDown;
}

void Hopper::setYDown(double newYDown)
{
    this->yDown = newYDown;
}

void Hopper::setZDown(double newZDown)
{
    this->zDown = newZDown;
}

void Hopper::setVx(double newVx)
{
    this->vx = newVx;
}

void Hopper::setVy(double newVy)
{
    this->vy = newVy;
}

void Hopper::setVz(double newVz)
{
    this->vz = newVz;
}

void Hopper::setAlfa(double newAlfa)
{
    this->alfa = newAlfa;
}

void Hopper::setLength(double newLength)
{
    this->length = newLength;
}

void Hopper::setBeta(double newBeta)
{
    this->beta = newBeta;
}

void Hopper::setVLength(double newVLength)
{
    this->vLength = newVLength;
}

void Hopper::setVAlfa(double newVAlfa)
{
    this->vAlfa = newVAlfa;
}

void Hopper::setVBeta(double newVBeta)
{
    this->vBeta = newVBeta;
}

void Hopper::setALength(double newALength)
{
    this->aLength = newALength;
}

void Hopper::setAAlfa(double newAAlfa)
{
    this->aAlfa = newAAlfa;
}

void Hopper::setABeta(double newABeta)
{
    this->aBeta = newABeta;
}

void Hopper::setEk(double newEk)
{
    this->ek = newEk;
}

void Hopper::setEph(double newEph)
{
    this->eph = newEph;
}

void Hopper::setEps(double newEps)
{
    this->eps = newEps;
}

double Hopper::getX()
{
    return this->x;
}

double Hopper::getY()
{
    return this->y;
}

double Hopper::getZ()
{
    return this->z;
}

double Hopper::getXDown()
{
    return this->xDown;
}

double Hopper::getYDown()
{
    return this->yDown;
}

double Hopper::getZDown()
{
    return this->zDown;
}

double Hopper::getAlfa()
{
    return this->alfa;
}

double Hopper::getBeta()
{
    return this->beta;
}

double Hopper::getVy()
{
    return this->vy;
}

double Hopper::getVx()
{
    return this->vx;
}

double Hopper::getVz()
{
    return this->vz;
}

double Hopper::getLength()
{
    return this->length;
}

double Hopper::getVLength()
{
    return this->vLength;
}

double Hopper::getVAlfa()
{
    return this->vAlfa;
}

double Hopper::getVBeta()
{
    return this->vBeta;
}

double Hopper::getALength()
{
    return this->aLength;
}

double Hopper::getAAlfa()
{
    return this->aAlfa;
}

double Hopper::getABeta()
{
    return this->aBeta;
}

double Hopper::getEk()
{
    return this->ek;
}

double Hopper::getEph()
{
    return this->eph;
}

double Hopper::getEps()
{
    return this->eps;
}

