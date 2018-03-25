/***********************************************************
	
	Starter code for Assignment 3

	Implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"

void PointLight::shade(Ray3D& ray) {
	// TODO: implement this function to fill in values for ray.col 
	// using phong shading.  Make sure your vectors are normalized, and
	// clamp colour values to 1.0.
	//
	// It is assumed at this point that the intersection information in ray 
	// is available.  So be sure that traverseScene() is called on the ray 
	// before this function. 

	// Find the position of the light from the intersection position.
	Vector3D lightDirection = 
			((this -> pos) - ray.intersection.point);
	lightDirection.normalize();

	// in [0,1]
	double diffuseIntensity = 
			std::max(0.0, ray.intersection.normal.dot(lightDirection));

	Vector3D mirrorRay = (
			(2.0 * ray.intersection.normal.dot(lightDirection) * ray.intersection.normal)
			 - lightDirection
	);
	mirrorRay.normalize();

	// Just in case
	ray.dir.normalize();

	double specularIntensity = pow(
			std::max(0.0, ray.dir.dot(-1 * mirrorRay)), 
			ray.intersection.mat -> specular_exp
	);

	// todo ADD material coloring

	Color diffuseColor = diffuseIntensity * this -> col_diffuse;
	diffuseColor.clamp();
	Color specularColor = specularIntensity * this -> col_specular;
	specularColor.clamp();

	ray.col = (
			(this -> col_ambient * ray.intersection.mat -> ambient) + 
			(diffuseColor * ray.intersection.mat -> diffuse) + 
			(specularColor * ray.intersection.mat -> specular)
	);
	ray.col.clamp();

}

