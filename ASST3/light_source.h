/***********************************************************
	
	Starter code for Assignment 3

	Light source classes

***********************************************************/
#pragma once

#include "util.h"
#include <vector>

#ifndef PI
#define PI 3.14159265358979323846
#endif
// Base class for a light source.  You could define different types
// of lights here, but point light is sufficient for most scenes you
// might want to render.  Different light sources shade the ray 
// differently.
class LightSource {
public:
	virtual void shade(Ray3D&) = 0;
	virtual Point3D get_position() const = 0; 
	virtual ~LightSource() {}
	virtual Color get_ambient() = 0;
	virtual std::vector<Ray3D*> generateSamples(Ray3D&) = 0;
};  

// List of all light sources in your scene
typedef std::vector<LightSource*> LightList;

// A point light is defined by its position in world space and its
// color.
class PointLight : public LightSource {
public:
	PointLight(Point3D pos, Color col) 
	: 
	pos(pos), col_ambient(0.2 * col), col_diffuse(col), col_specular(col) {}
	
	PointLight(Point3D pos, Color ambient, Color diffuse, Color specular) 
	: 
	pos(pos), col_ambient(ambient), col_diffuse(diffuse), col_specular(specular) {}
	
	void shade(Ray3D& ray);
	
	Point3D get_position() const { return pos; }

	Color get_ambient();

	std::vector<Ray3D*> generateSamples(Ray3D&);
	
private:
	Point3D pos;
	Color col_ambient;
	Color col_diffuse; 
	Color col_specular; 
};

class ExtendedPointLight : public LightSource {
public:
	ExtendedPointLight(Point3D pos, Color col, double radius) 
	: 
	pos(pos), col_ambient(0.2 * col), col_diffuse(col), col_specular(col), radius(radius) {}
	
	ExtendedPointLight(Point3D pos, Color ambient, Color diffuse, Color specular, double radius) 
	: 
	pos(pos), col_ambient(ambient), col_diffuse(diffuse), col_specular(specular), radius(radius) {}
	
	void shade(Ray3D& ray);
	
	Point3D get_position() const { return pos; }

	Color get_ambient();

	std::vector<Ray3D*> generateSamples(Ray3D&);
	
private:
	double radius;
	Point3D pos;
	Color col_ambient;
	Color col_diffuse; 
	Color col_specular; 
};

Color Ambient(LightList& lights, Material* mat);
