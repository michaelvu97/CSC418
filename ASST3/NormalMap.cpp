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

Vector3D LumpyNormal::bump(const Point3D& point) {

    Vector3D offset(0,0,0);

    double cosx = cos(2 * PI * this -> period * point[0]);
    double cosy = cos(2 * PI * this -> period * point[1]);


    if (cosx * cosy < this -> depthThreshold)
        return offset;

    offset[0] = sin(2 * PI * this -> period * point[0]);
    offset[1] = sin(2 * PI * this -> period * point[1]);

    return this -> magnitude * (cosx < 0 ? -1 : 1) * offset;

}

Vector3D RibbedNormal::bump (const Point3D& point) {
    
    double cosx = cos(2 * PI * this -> period * point[0]);

    double cosx4 = pow(cosx, 4);

    if (cosx4 > this -> heightCutoff) {
        return Vector3D(0, 0, 0);
    }

    // Derivative.
    return this -> magnitude * Vector3D(4 * pow(cosx, 3) * sin(point[0]), 0, 0);

}

Vector3D PolynomialNoiseNormal::bump (const Point3D& point) {
    double xheight = 0.0;
    double yheight = 0.0;

    for (int i = 0; i < this -> order; i++) {
        xheight += (i % 2 == 0 ? 1 : -1) * 
                sin(PI * point[0] * this -> xroots[i] + this -> xoffsets[i]);
        yheight += (i % 2 == 0 ? 1: -1) *
                sin(PI * point[1] * this -> yroots[i] + this -> yoffsets[i]);
    }

    return (1.0  / (this -> order)) * Vector3D(xheight, yheight, 0);
}

Vector3D MetallicGrainNormal::bump(const Point3D& point) {

    bool hasBeenSet = false;
    Vector3D offset(0, 0, 0);

    for (int i = 0; i < this -> dentLocations.size(); i++) {
        Vector3D delta = (Vector3D(point[0], point[1], 0) - this -> dentLocations[i]);

        double deltaLenSquared = delta[0] * delta[0] + delta[1] * delta[1];

        if (!hasBeenSet) {
            offset = -delta;
            hasBeenSet = true;
        } else {
            if (deltaLenSquared < pow(offset[0], 2) + pow(offset[1], 2)) {
                offset = delta;
            }
        }

    }

    return offset;

}