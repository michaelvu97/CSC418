/***********************************************************
	
	Starter code for Assignment 3
	
	Implements scene_object.h

***********************************************************/

#include <cmath>
#include "scene_object.h"
#include "stdlib.h"

bool UnitSquare::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld) {
	// TODO: implement intersection code for UnitSquare, which is
	// defined on the xy-plane, with vertices (0.5, 0.5, 0), 
	// (-0.5, 0.5, 0), (-0.5, -0.5, 0), (0.5, -0.5, 0), and normal
	// (0, 0, 1).
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.

	// Copy the ray to transform to model space
	Ray3D rayModelSpace;
	rayModelSpace.origin = worldToModel * ray.origin;
	rayModelSpace.dir = worldToModel * ray.dir;

	// Find the ray parameter/position at the plane.
	double t = -rayModelSpace.origin[2] / rayModelSpace.dir[2];

	// t must be positive.
	if (t < EPSILON) {
		// ray.intersection.none = true;
		return false;
	}

	// Compute the ray's xy-position on the plane.
	Point3D rayPosOnPlane = rayModelSpace.origin + (t * rayModelSpace.dir);

	double x = rayPosOnPlane[0];
	double y = rayPosOnPlane[1];

	if (x >= - 0.5 && x <= 0.5 && y >= -0.5 && y <= 0.5) {

		

		if ((t < ray.intersection.t_value && !ray.intersection.none) ||
			ray.intersection.none) {
			ray.intersection.point = modelToWorld * (rayPosOnPlane);
			ray.intersection.normal = 
					(worldToModel.transpose() * Vector3D(0,0,1));
			ray.intersection.normal.normalize();
			ray.intersection.none = false;
			ray.intersection.t_value = t;
			return true;
		}
	}
	
	return false;
	

}

bool UnitSphere::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld) {
	// TODO: implement intersection code for UnitSphere, which is centred 
	// on the origin.  
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.
	Ray3D rayModelSpace;
	rayModelSpace.origin = worldToModel * ray.origin;
	rayModelSpace.dir = worldToModel * ray.dir;

	double* intersections = (double *) malloc(sizeof(double) * 2);

	int nIntersections = SolveQuadratic(
			rayModelSpace.dir.dot(rayModelSpace.dir),
			2.0 * rayModelSpace.dir.dot(rayModelSpace.origin.ToVector()),
			(rayModelSpace.origin.ToVector().dot(rayModelSpace.origin.ToVector())) - 1,
			intersections
	);

	Point3D rayPosOnSphere;

	if (nIntersections == 0) {
		goto no_intersections;
	}


	double t;

	if (nIntersections == 1) {

		// Only accept positive t
		if (intersections[0] < EPSILON) {
			goto no_intersections;
		}

		t = intersections[0];

	}

	if (nIntersections == 2) {

		/*
		 * Find the closest to the camera, given by the intersection with the
		 * smallest magnitude
		 */
		if (intersections[0] < EPSILON) {

			if (intersections[1] < EPSILON) {
				goto no_intersections;
			}

			// Pick 1
			t = intersections[1];


		} else if (intersections[1] < EPSILON) {

			// pick 0
			t = intersections[0];

		} else {
			// pick the closest to the camera
			t = std::min(intersections[0],intersections[1]);
		}
	}

	free(intersections);


	// Valid intersection
	if ((t < ray.intersection.t_value && !ray.intersection.none) ||
			ray.intersection.none) {
		rayPosOnSphere = rayModelSpace.origin + (t * rayModelSpace.dir);
		ray.intersection.point = modelToWorld * (rayPosOnSphere);
		ray.intersection.normal = (worldToModel.transpose() * 
				(rayPosOnSphere.ToVector()));
		ray.intersection.normal.normalize();
		ray.intersection.t_value = t;

		ray.intersection.none = false;
		return true;
	}

	return false;

no_intersections:
	free(intersections);
	return false;
}

void SceneNode::rotate(char axis, double angle) {
	Matrix4x4 rotation;
	double toRadian = 2*M_PI/360.0;
	int i;
	
	for (i = 0; i < 2; i++) {
		switch(axis) {
			case 'x':
				rotation[0][0] = 1;
				rotation[1][1] = cos(angle*toRadian);
				rotation[1][2] = -sin(angle*toRadian);
				rotation[2][1] = sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'y':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][2] = sin(angle*toRadian);
				rotation[1][1] = 1;
				rotation[2][0] = -sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'z':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][1] = -sin(angle*toRadian);
				rotation[1][0] = sin(angle*toRadian);
				rotation[1][1] = cos(angle*toRadian);
				rotation[2][2] = 1;
				rotation[3][3] = 1;
			break;
		}
		if (i == 0) {
			this->trans = this->trans*rotation; 	
			angle = -angle;
		} 
		else {
			this->invtrans = rotation*this->invtrans; 
		}	
	}
}

void SceneNode::translate(Vector3D trans) {
	Matrix4x4 translation;
	
	translation[0][3] = trans[0];
	translation[1][3] = trans[1];
	translation[2][3] = trans[2];
	this->trans = this->trans*translation; 	
	translation[0][3] = -trans[0];
	translation[1][3] = -trans[1];
	translation[2][3] = -trans[2];
	this->invtrans = translation*this->invtrans; 
}

void SceneNode::scale(Point3D origin, double factor[3] ) {
	Matrix4x4 scale;
	
	scale[0][0] = factor[0];
	scale[0][3] = origin[0] - factor[0] * origin[0];
	scale[1][1] = factor[1];
	scale[1][3] = origin[1] - factor[1] * origin[1];
	scale[2][2] = factor[2];
	scale[2][3] = origin[2] - factor[2] * origin[2];
	this->trans = this->trans*scale; 	
	scale[0][0] = 1/factor[0];
	scale[0][3] = origin[0] - 1/factor[0] * origin[0];
	scale[1][1] = 1/factor[1];
	scale[1][3] = origin[1] - 1/factor[1] * origin[1];
	scale[2][2] = 1/factor[2];
	scale[2][3] = origin[2] - 1/factor[2] * origin[2];
	this->invtrans = scale*this->invtrans; 
}


