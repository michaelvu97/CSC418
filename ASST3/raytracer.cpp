/***********************************************************
	
	Starter code for Assignment 3

	Implementations of functions in raytracer.h, 
	and the main function which specifies the scene to be rendered.	

***********************************************************/


#include "raytracer.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <float.h>
#include <vector>

void Raytracer::traverseScene(Scene& scene, Ray3D& ray)  {
	for (size_t i = 0; i < scene.size(); ++i) {
		SceneNode* node = scene[i];

		if (node->obj->intersect(ray, node->worldToModel, node->modelToWorld)) {
			ray.intersection.mat = node->mat;
	 	}
	}
}

void Raytracer::computeTransforms(Scene& scene) {
	// right now this method might seem redundant. But if you decide to implement 
	// scene graph this is where you would propagate transformations to child nodes
		
	for (size_t i = 0; i < scene.size(); ++i) {
		SceneNode* node = scene[i];

		node->modelToWorld = node->trans;
		node->worldToModel = node->invtrans; 
	}
}

void Raytracer::computeShading(Ray3D& ray, LightSource* light) {
	light -> shade(ray);
}

void Raytracer::computeShading(Ray3D& ray, LightList& light_list) {
	for (size_t  i = 0; i < light_list.size(); ++i) {
		LightSource* light = light_list[i];
		
		// Each lightSource provides its own shading function.
		// Implement shadows here if needed.
		light->shade(ray);        
	}
}

Color Raytracer::shadeRay(Ray3D& ray, Scene& scene, LightList& light_list, int depth, int gatherAmbient) {

	Color col(0.0, 0.0, 0.0); 

	// if (depth == 0) {
	// 	// Do something here, we are done raytracing.
	// 	computeShading(ray, light_list);
	// 	col = col + ray.col;  
	// 	col.clamp();
	// 	return col;
	// }

	traverseScene(scene, ray); 

	// Don't bother shading if the ray didn't hit 
	// anything.
	if (!ray.intersection.none) {

		

		// Create new ray
		if (depth > 0) {
			Ray3D rayReflection;
			rayReflection.origin = ray.intersection.point;
			rayReflection.dir = 
					ray.dir - 	
					(2.0 * (ray.intersection.normal.dot(ray.dir)) * 
					ray.intersection.normal);
			rayReflection.dir.normalize();

			// Create a "fake" light source from the reflected ray.
			Color c = shadeRay(rayReflection, scene, light_list, depth - 1, 0);

			// Calculate phong using this new light.

			Color blankColor(0.0, 0.0, 0.0);
			col = col + Phong(rayReflection.dir, ray.intersection.normal,
					ray.dir, ray.intersection.mat, blankColor, blankColor, c);
			col.clamp();
		}

		// std::vector<Ray3D> raysToLights;

		for (int i = 0; i < light_list.size(); i++) {
			Ray3D newLightRay;
			newLightRay.origin = ray.intersection.point;
			newLightRay.dir = (light_list[i] -> get_position()) - newLightRay.origin;
			newLightRay.dir.normalize();

			traverseScene(scene, newLightRay);

			if (newLightRay.intersection.none) {
				// The light was unobstructed
				computeShading(ray, light_list[i]);
				col = col + ray.col;
				col.clamp();
			} else if (gatherAmbient) {
				// Just use light ambient.
				col = col + Ambient(light_list, ray.intersection.mat);
			}

		}

		
	}

	// You'll want to call shadeRay recursively (with a different ray, 
	// of course) here to implement reflection/refraction effects.  
	col.clamp();
	return col; 
}	

void Raytracer::render(Camera& camera, Scene& scene, LightList& light_list, Image& image) {
	computeTransforms(scene);

	Matrix4x4 viewToWorld;
	double factor = (double(image.height)/2)/tan(camera.fov*M_PI/360.0);

	viewToWorld = camera.initInvViewMatrix();

	// Construct a ray for each pixel.
	for (int i = 0; i < image.height; i++) {
		for (int j = 0; j < image.width; j++) {
			// Sets up ray origin and direction in view space, 
			// image plane is at z = -1.
			Point3D origin(0, 0, 0);
			Point3D imagePlane;
			imagePlane[0] = ( -double(image.width)/2 + 0.5 + j) / factor;
			imagePlane[1] = ( -double(image.height)/2 + 0.5 + i) / factor;
			imagePlane[2] = -1; // Camera depth
			
			Ray3D ray;
			// TODO: Convert ray to world space  
			ray.origin = camera.eye;
			ray.dir = ((viewToWorld * imagePlane) - camera.eye);
			ray.dir.normalize();
			ray.intersection.t_value = DBL_MAX;

			Color col = shadeRay(ray, scene, light_list, 3, 1); 
			image.setColorAtPixel(i, j, col);			
		}
	}
}

