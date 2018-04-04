#include "util.h"
#include <cmath>
#include <vector>

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
    /*
     * Creates single-direction cosine waves.
     */
public:

    Vector3D bump(const Point3D& p);

    double period = 24.0;
    double phaseOffset = 0.0;
    double magnitude = 0.25;

};

class RadialCorrugatedNormal: public NormalMap {
    /*
     * Creates radial cosine waves.
     */
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


class NoiseyNormal: public NormalMap {
    /*
     * Creates completely noisey surface normal
     */
public:

    NoiseyNormal(double magnitude) {
        this -> magnitude = magnitude;
    }

    Vector3D bump(const Point3D& point);

    double magnitude = 0.2;

};

class BricksNormal: public NormalMap {
    /* 
     * Creates kind of a brick effect.
     */
public:

    Vector3D bump(const Point3D& point);

    double xPeriod = 6.0;
    double yPeriod = 6.0;
    double magnitude = 0.2;
    double depthThreshold = -0.9;

};

class LumpyNormal : public NormalMap {
    /*
     * Small round bumps across the surface.
     */
public:

    Vector3D bump (const Point3D& point);

    double period = 6.0;
    double depthThreshold = 0.8;
    double magnitude = 10;
};

class RibbedNormal : public NormalMap {
    /* 
     * For her pleasure
     */

public:
    Vector3D bump (const Point3D& point);

    double period = 6.0;
    double heightCutoff = 0.58;
    double magnitude = 0.3;
};

class PolynomialNoiseNormal : public NormalMap {
    /*
     * Generates a random harmonic noise map.
     * Benefits are: computation time, continuousness.
     */
public:
    PolynomialNoiseNormal(int order) {

        int noiseLevel = order * 2;

        for (int i = 0; i < order; i++) {
            this -> xroots.push_back(double(rand() % noiseLevel) - double(noiseLevel) / 2);
            this -> yroots.push_back(double(rand() % noiseLevel) - double(noiseLevel) / 2);
            this -> xoffsets.push_back(double(rand() % noiseLevel) - double(noiseLevel) / 2);
            this -> yoffsets.push_back(double(rand() % noiseLevel) - double(noiseLevel) / 2);
        }

        this -> order = order;

    }

    Vector3D bump (const Point3D& point);

private:

    // Roots in [0,1]
    int order;
    std::vector<double> xroots;
    std::vector<double> yroots;

    std::vector<double> xoffsets;
    std::vector<double> yoffsets;

};

class MetallicGrainNormal : public NormalMap {
    /*
     * Simulates metal surface grains, essentially k-nn voronoi.
     */

public:
    MetallicGrainNormal(int nDents) {


        for (int i = 0; i < nDents; i++) {
            dentLocations.push_back(Vector3D(
                    (rand() % 50) / 50.0,
                    (rand() % 50) / 50.0,
                    0.0
            )); 

        }

    }

    Vector3D bump (const Point3D& point);

private:
    std::vector<Vector3D> dentLocations;
};


#endif