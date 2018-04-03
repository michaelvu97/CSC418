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
    Vector3D bump(const Point3D& p) {
        return Vector3D(
                this -> magnitude * 
                        sin(this -> phaseOffset + this -> period * PI * p[0]), 
                0, 
                0
        );
    }
    double period = 24.0;
    double phaseOffset = 0.0;
    double magnitude = 0.25;
};

class RadialCorrugatedNormal: public NormalMap {
public:
    RadialCorrugatedNormal(double x, double y) {
        this -> centre = Point3D(x, y, 0);
    }
    Vector3D bump (const Point3D& p) {

        Vector3D delta = p - this -> centre;

        double res = 
                this -> magnitude * 
                sin(
                        this -> period * PI * delta.length() + 
                        this -> phaseOffset
                );

        delta.normalize();
    
        return res * delta;
    }
    double period = 24.0;
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
    Vector3D bump(const Point3D& point) {
        return this -> magnitude * Vector3D(
                ((rand() % 50) - 25.0) / 50,
                ((rand() % 50) - 25.0) / 50,
                0.0
        );
    }
    double magnitude = 0.2;
};

#endif