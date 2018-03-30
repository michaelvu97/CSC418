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

std::vector<Ray3D*> PointLight::generateSamples(Ray3D& ray) {
	std::vector<Ray3D*> retVal;


	Ray3D* principleRay = new Ray3D();
	principleRay -> origin = ray.origin;
	principleRay -> dir = ray.dir;

	// Push principle ray
	retVal.push_back(principleRay);
	return retVal;
}

std::vector<Ray3D*> ExtendedPointLight::generateSamples(Ray3D& ray) {

	std::vector<Ray3D*> retVal;

	Ray3D* principleRay = new Ray3D();
	principleRay -> origin = ray.origin;
	principleRay -> dir = ray.dir;

	// Push principle ray
	retVal.push_back(principleRay);


	// Vector3D deltaPos = (ray.origin - this -> pos);
	// int nSamples = (int) (
	// 		AREA_LIGHT_SAMPLES * PI * pow(this -> radius, 2) / 
	// 		(deltaPos.dot(deltaPos))
	// );

	// if (nSamples < 1) {
	// 	nSamples = 1;
	// }

	// for (int i = 0; i < nSamples - 1; i++) {
	// 	// light direction is randomly pick over a radius
	// 	double r = radius * (rand() % 50) / 50.0;
	// 	double theta = 2 * PI * (rand() % 50) / 50.0;
	// 	double phi = 2* PI * (rand() % 50) / 50.0;

	// 	Point3D randLightPos = this-> pos + Vector3D(r* cos(theta)*sin(phi),
	// 			r * sin(theta)*sin(phi),
	// 			r * cos(phi));

	// 	Vector3D lightDirection = 
	// 			(randLightPos - ray.origin);

	// 	lightDirection.normalize();

	// 	Ray3D* newRay = new Ray3D();
	// 	newRay -> origin = ray.origin;
	// 	newRay -> dir = lightDirection;
	// 	retVal.push_back(newRay);
	// }

	if (SOFT_SHADOWS_ENABLE) {

		double k = SOFT_SHADOWS_DELTA;

		double dr = (this -> radius) / k;
		double dtheta = 2 * PI / k;
		double dphi = PI / k;

		for (double r = dr; r < this -> radius; r += dr) {
			for (double theta = dtheta; theta < 2 * PI; theta += dtheta) {
				for (double phi = dphi; phi < PI; phi += dphi) {

					Point3D randLightPos = this-> pos + Vector3D(r* cos(theta)*sin(phi),
					r * sin(theta)*sin(phi),
					r * cos(phi));

					Vector3D lightDirection = 
							(randLightPos - ray.origin);

					lightDirection.normalize();

					Ray3D* newRay = new Ray3D();
					newRay -> origin = ray.origin;
					newRay -> dir = lightDirection;
					retVal.push_back(newRay);
				}
			}
		}
	}
	
	return retVal;
}

void ExtendedPointLight::shade(Ray3D& ray){
	
	Vector3D lightDirection = (this->pos) - ray.intersection.point;
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
