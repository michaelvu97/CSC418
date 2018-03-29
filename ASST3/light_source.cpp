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
	// TODO use phong()
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

	Color diffuseColor = diffuseIntensity * this -> col_diffuse;
	diffuseColor.clamp();
	Color specularColor = specularIntensity * this -> col_specular;
	specularColor.clamp();

	// TODO: prevent light source overwrite.
	ray.col = (
			(this -> col_ambient * ray.intersection.mat -> ambient) + 
			(diffuseColor * ray.intersection.mat -> diffuse) + 
			(specularColor * ray.intersection.mat -> specular)
	);
	ray.col.clamp();

}

Color Ambient(LightList& lights, Material* mat) {
	Color c(0.0,0.0,0.0);
	for (int i = 0 ; i < lights.size(); i++) {
		c = c + ((lights[i] -> get_ambient()) * mat -> ambient);
	}
	c.clamp();
	return c;

}

Color PointLight::get_ambient() {
	return this -> col_ambient;
}

void ExtendedPointLight::shade(Ray3D& ray){
	// light direction is randomly pick over a radius
	double r = RADIUS * rand();
	double theta = 2 * PI * rand();
	double phi = 2* PI * rand();

	Point3D randLightPos = this-> pos + Vector3D(r* cos(theta)*sin(phi),
			r * sin(theta)*sin(phi),
			r * cos(phi));

	Vector3D lightDirection = 
			(randLightPos - ray.intersection.point);
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

	Color diffuseColor = diffuseIntensity * this -> col_diffuse;
	diffuseColor.clamp();
	Color specularColor = specularIntensity * this -> col_specular;
	specularColor.clamp();

	// TODO: prevent light source overwrite.
	ray.col = (
			(this -> col_ambient * ray.intersection.mat -> ambient) + 
			(diffuseColor * ray.intersection.mat -> diffuse) + 
			(specularColor * ray.intersection.mat -> specular)
	);
	ray.col.clamp();




}
Color ExtendedPointLight::get_ambient() {
	return this -> col_ambient;
}
