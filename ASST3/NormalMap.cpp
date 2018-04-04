#include "NormalMap.h"

Vector3D CorrugatedNormal::bump(const Point3D& p) {
    return Vector3D(
            this -> magnitude * 
                    sin(this -> phaseOffset + this -> period * PI * p[0]), 
            0, 
            0
    );
}

Vector3D RadialCorrugatedNormal::bump (const Point3D& p) {

    Vector3D delta = p - this -> centre;

    double res = 
            this -> magnitude * 
            sin(
                    this -> period * 2 *  PI * delta.length() + 
                    this -> phaseOffset
            );

    delta.normalize();

    return res * delta;
}

Vector3D NoiseyNormal::bump(const Point3D& point) {
    return this -> magnitude * Vector3D(
            ((rand() % 50) - 25.0) / 50,
            ((rand() % 50) - 25.0) / 50,
            0.0
    );
}

Vector3D BricksNormal::bump(const Point3D& point) {

        double cosx = cos(2 * PI * this -> xPeriod * point[0]);

        Vector3D offset(0, 0, 0);

        double dx = 0;
        double dy = 0;

        bool dxSet = false, dySet = false;

        if (cosx <= depthThreshold) {
            double sinx = sin(2 * PI * this -> xPeriod * point[0]);
            dx = sinx * magnitude;
            dxSet = true;
        }

        double whichPhase = cosx > 0 ? -1 : 1;

        double cosy1 = cos(2 * PI * this -> yPeriod * point[1]);

        if (cosy1 <= depthThreshold) {
            double siny = sin(2 * PI * this -> yPeriod * point[1]);
            dy = siny * magnitude;
            dySet = true;
        } 

        if (dySet && dxSet) {
            // Pick the minimum of the two slopes.
            if (abs(dy) < abs(dx)) {
                offset[1] = dy;
            } else {
                offset[0] = dx;
            }
        } else {
            offset[0] = dx;
            offset[1] = dy;
        }

        return offset;

    }