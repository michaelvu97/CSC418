#include "util.h"
#include <cmath>

#ifndef PI
#define PI 3.14159265358979323846
#endif

#ifndef NORMALMAP_H
#define NORMALMAP_H

class NormalMap {
public:
    // Returns a delta x, delta y for the bump map. Point is given in object space.
    virtual Vector3D bump(const Point3D& p) = 0;    
};

class CorrugatedNormal: public NormalMap {

public:

    Vector3D bump(const Point3D& p);

    double period = 24.0;
    double phaseOffset = 0.0;
    double magnitude = 0.25;

};

class RadialCorrugatedNormal: public NormalMap {

public:

    RadialCorrugatedNormal(double x, double y) {
        this -> centre = Point3D(x, y, 0);
    }

    Vector3D bump (const Point3D& p);

    double period = 12.0;
    double phaseOffset = 0.0;
    double magnitude = 0.25;

private:

    Point3D centre;

};

/*
 * Creates completely noisey surface normal
 */
class NoiseyNormal: public NormalMap {

public:

    NoiseyNormal(double magnitude) {
        this -> magnitude = magnitude;
    }

    Vector3D bump(const Point3D& point);

    double magnitude = 0.2;

};

class BricksNormal: public NormalMap {

public:

    Vector3D bump(const Point3D& point);

    double xPeriod = 6.0;
    double yPeriod = 6.0;
    double magnitude = 0.2;
    double depthThreshold = -0.9;

};

#endif