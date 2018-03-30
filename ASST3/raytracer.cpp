/***********************************************************
	
	Starter code for Assignment 3

	Implementations of functions in raytracer.h, 
	and the main function which specifies the scene to be rendered.	

***********************************************************/

#include <limits>
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

Color Raytracer::shadeRay(Ray3D& ray, Scene& scene, LightList& light_list, 
		int depth, int gatherAmbient) {

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
			//create a cone of rays
			double g = ray.intersection.mat -> glossiness;

			Vector3D deltaYVec = ray.dir.cross(ray.intersection.normal);

			if (deltaYVec.length() <= std::numeric_limits<double>::epsilon()) {
				/* 
				 * This is a really rare corner case where the ray is parallel
				 * to the normal. Handle accordingly
				 */

				Vector3D testVec(0.0, 0.0, 0.1);

				std::cout << "found a normal in the rare case\n";

				deltaYVec = ray.dir.cross(testVec);

				if (deltaYVec.length() <= 
						std::numeric_limits<double>::epsilon()) {

					testVec[0] = 0.0;
					testVec[1] = 1.0;
					testVec[2] = 0.0;

					deltaYVec = ray.dir.cross(testVec);

				}

			} 

			Vector3D deltaXVec = ray.dir.cross(deltaYVec);

			deltaYVec.normalize();
			deltaXVec.normalize();

			Color colorSum(0.0, 0.0, 0.0);

			// Must be greater than 0, more shells is more gloss rays.
			int shells = DEFAULT_GLOSS_SHELLS;

			if (shells > 0) {

				// ISSUE TODO: gloss is allowing some rays to pass through and 
				// see the backface.

				int ignoredRays = 0;

				for (double dx = -g; dx <= g; dx += g / shells) {
					for (double dy = -g; dy <= g; dy +=  g / shells) {
						Ray3D tempRay;
						tempRay.origin = ray.intersection.point;
						tempRay.dir = rayReflection.dir + (dx * deltaXVec) + 
								(dy * deltaYVec);

						tempRay.dir.normalize();

						if (tempRay.dir.dot(ray.intersection.normal) < 0) {
							ignoredRays++;
							continue;
						}

						Color c = shadeRay(tempRay, scene, light_list, depth - 1,
								0);

						colorSum = colorSum + 
								(pow(tempRay.dir.dot(rayReflection.dir),
								ray.intersection.mat -> specular_exp) * c);
					}
				}

				// Average
				colorSum = ( GLOSS_BRIGHTNESS_MULTIPLIER / 
						(pow(1.0 + 2.0 * shells, 2) - ignoredRays) ) * colorSum;
			} else {
				colorSum = shadeRay(rayReflection, scene, light_list, 
					depth - 1, 0);
			}

			Color blankColor(0.0, 0.0, 0.0);
			col = col + Phong(rayReflection.dir, ray.intersection.normal,
					ray.dir, ray.intersection.mat, blankColor, blankColor, 
					colorSum);
			
			col.clamp();

		}

		for (int i = 0; i < light_list.size(); i++) {

			Ray3D newLightRay;
			newLightRay.origin = ray.intersection.point;
			newLightRay.dir = 
					(light_list[i] -> get_position()) - newLightRay.origin;
			newLightRay.dir.normalize();

			std::vector<Ray3D*> samples = 
					light_list[i] -> generateSamples(newLightRay);

			Color colorSum(0, 0, 0);
			
			int hits = 0;

			// TODO glossiness.

			for (int sampleNum = 0; sampleNum < samples.size(); sampleNum++) {

				// TODO allow a limit on traverse Scene dist.
				traverseScene(scene, *samples[sampleNum]); 

				if (samples[sampleNum] -> intersection.none) {
					// The light was unobstructed
					computeShading(ray, light_list[i]);
					hits++;
					colorSum = colorSum + ray.col;
				}

				delete samples[sampleNum];
			}


			if (hits == 0 && gatherAmbient) {
				// Just use light ambient.
				col = col + Ambient(light_list, ray.intersection.mat);
			} else {
				col =  col + (1.0 / (double) samples.size()) * colorSum;
			}

			

		}

		
	}

	// You'll want to call shadeRay recursively (with a different ray, 
	// of course) here to implement reflection/refraction effects.  
	col.clamp();
	return col; 
}	

void Raytracer::render(Camera& camera, Scene& scene, LightList& light_list, 
		Image& image) {

	computeTransforms(scene);

	Matrix4x4 viewToWorld;
	double factor = (double(image.height)/2)/tan(camera.fov*M_PI/360.0);

	viewToWorld = camera.initInvViewMatrix();

	// Construct a ray for each pixel.
	for (int i = 0; i < image.height; i++) {

		//std::cout << double(i) * 100.0 / image.height << "%\n";

		for (int j = 0; j < image.width; j++) {
			// Sets up ray origin and direction in view space, 
			// image plane is at z = -1.

			Point3D origin(0, 0, 0);
			Point3D imagePlane;
			imagePlane[0] = ( -double(image.width)/2 + 0.5 + j) / factor;
			imagePlane[1] = ( -double(image.height)/2 + 0.5 + i) / factor;
			imagePlane[2] = -1; // Camera depth
			
			Ray3D ray;
			ray.origin = camera.eye;
			ray.dir = ((viewToWorld * imagePlane) - camera.eye);
			ray.dir.normalize();
			ray.intersection.t_value = DBL_MAX;

			Color colCentre = shadeRay(ray, scene, light_list, RAY_TRACE_DEPTH, 
					1); 

			if (ANTI_ALIASING_ENABLED) {

				for (	double dx = -ANTI_ALIASING_DELTA; 
						dx <= ANTI_ALIASING_DELTA; 
						dx += 2 * ANTI_ALIASING_DELTA) {

					for (	double dy = -ANTI_ALIASING_DELTA; 
							dy <= ANTI_ALIASING_DELTA; 
							dy += 2 * ANTI_ALIASING_DELTA) {

						Point3D imagePlaneAA;

						imagePlaneAA[0] = 
								(-double(image.width)/2 + j + 0.5 + dx) 
								/ factor;

						imagePlaneAA[1] = 
								(-double(image.height)/2 + i + 0.5 + dy)
								/ factor;

						imagePlaneAA[2] = -1; // Camera depth

						Ray3D ray2;
						ray2.origin = camera.eye;
						ray2.dir = ((viewToWorld * imagePlaneAA) - camera.eye);
						ray2.dir.normalize();
						ray2.intersection.t_value = DBL_MAX;

						Color aa_res = shadeRay(ray2, scene, light_list, 2, 1);
					 	colCentre = colCentre + aa_res; 

					}
				}



				colCentre = 0.2 * colCentre; // Divide by 5 points.
				colCentre.clamp();



			}

			if(DOF_ENABLE) {
				//generate focal point for differnet ray dir
				Point3D focalPoint = ray.origin + FOCAL_LENGTH * ray.dir;
				//std::cout<< focalPoint << "\n";
				for(int i =0; i < 15; i++){
					//randomly pick points within the radius of the aperture 
					Vector3D tempVec (std::fmod(rand(), APERTURE), std::fmod(rand(), APERTURE), 0);
					Point3D randPoint = ray.origin + tempVec;
					//std::cout << randPoint << "\n";
					Ray3D secondaryRay;
					secondaryRay.origin = randPoint;
					secondaryRay.dir = focalPoint - randPoint;
					secondaryRay.dir.normalize();
					secondaryRay.intersection.t_value = DBL_MAX;

					Color dof_res = shadeRay(secondaryRay, scene, light_list, 2, 1);
					colCentre = colCentre + dof_res;

				}

				colCentre = 0.0667 * colCentre; // Divide by 15 points.
				colCentre.clamp();

			}

			image.setColorAtPixel(i, j, colCentre);			
		}
	}
}

