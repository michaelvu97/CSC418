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
#include <algorithm>

Color ambientLightColor;

void Raytracer::traverseScene(Scene& scene, Ray3D& ray, bool ignoreTransparent){
	for (size_t i = 0; i < scene.size(); ++i) {
		SceneNode* node = scene[i];

		if (ignoreTransparent && node -> mat -> opacity < 1.0 - EPSILON)
			continue;

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
		int depth, float index) {

	Color col(0.0, 0.0, 0.0); 

	// if (depth == 0) {
	// 	// Do something here, we are done raytracing.
	// 	computeShading(ray, light_list);
	// 	col = col + ray.col;  
	// 	col.clamp();
	// 	return col;
	// }

	traverseScene(scene, ray, false); 



	bool insideObject = index > AIR_REFRACTIVE + EPSILON;
	double reflectance;
	// Don't bother shading if the ray didn't hit 
	// anything.
	if (!ray.intersection.none) {
		// calculate frensnel
		// this is simply clamping between -1 and 1
		double cosi = std::max(-1.0, std::min(ray.dir.dot(ray.intersection.normal), 1.0));
		double ni = insideObject ? ray.intersection.mat->refraction_index : AIR_REFRACTIVE;
		double nt = insideObject ? AIR_REFRACTIVE : ray.intersection.mat ->refraction_index;
		double sint = (ni/nt) *sqrt(std::max(0.0, 1 - cosi * cosi));
		
		if(sint >= 1 ){
			//total internal reflection
			//reflectance = 1
			reflectance = 1;
			
		}
		else{
			//calculate frensnel 
			double cost = sqrt(std::max(0.0, 1- sint * sint));
			cosi = std::abs(cosi);
			double frensnel_TM = pow(((nt * cosi) - (ni * cost)) / ((nt * cosi) + (ni * cost)), 2.0);
			double frensnel_TE = pow(((ni * cost) - (nt * cosi)) / ((ni * cost) + (nt * cosi)), 2.0);
			reflectance = ((frensnel_TE + frensnel_TM) / 2.0);
			//this is a ratio
			if(reflectance > 1.0){
				std::cout<< reflectance << " ,"<< ni << " ," << nt << "\n";
			}
		}
		double transmittance = 1.0 - reflectance; 

		if(ray.intersection.mat->opacity == 1.0){
			//if it is opaque, reflectance tends to 1.0
			// std:: cout<< reflectance << "\n";
		}

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

			if (shells > 0 && g > EPSILON) {

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

						Color c = shadeRay(
								tempRay,
								scene,
								light_list, 
								depth - 1,
								index
						);

						double multiplier = pow(
								tempRay.dir.dot(rayReflection.dir),
								ray.intersection.mat -> specular_exp * g 
										* GLOSS_REGULARIZER
						);

						if (std::isnan(multiplier)){
							continue;
						}

						colorSum = colorSum + multiplier * c;

						colorSum.clamp();

					}
				}

				// Add the original ray.

				// Average
				colorSum = ( 1.0 / 
						(pow(1.0 + 2.0 * shells, 2) - ignoredRays) ) * colorSum;

				colorSum = colorSum + shadeRay(rayReflection, scene, light_list, 
					depth - 1, index);

			} else {
				colorSum = shadeRay(rayReflection, scene, light_list, 
					depth - 1, index);
			}

			colorSum.clamp();

			Color blankColor(0.0, 0.0, 0.0);

			double opacity = ray.intersection.mat -> opacity;
			double transparency = 1.0 - opacity;

			// Could potentially use ambientLightColor as the diffuse component to give more realistic result
			col = col + reflectance * 
					Phong(
						rayReflection.dir,
						ray.intersection.normal,
						ray.dir,
						ray.intersection.mat,
						ambientLightColor,
						blankColor,
						colorSum
					);
			// col = col + opacity * 
			// 		Phong(
			// 			rayReflection.dir,
			// 			ray.intersection.normal,
			// 			ray.dir,
			// 			ray.intersection.mat,
			// 			ambientLightColor,
			// 			blankColor,
			// 			colorSum
			// 		);
			
			col.clamp();
			
			//handle refraction

			if (transparency > EPSILON) {

				//generate refracted ray
				Ray3D refractedRay;
				refractedRay.origin = ray.intersection.point;
				float n1 = index;
				float n2 = insideObject ? 
						AIR_REFRACTIVE : ray.intersection.mat->refraction_index;
				float n = n1 / n2;

				double normalMultiplier = insideObject ? -1.0 : 1.0;

				double c1 = -1.0 * normalMultiplier * 
						(ray.intersection.normal.dot(ray.dir));

				double c2_inside = 1.0 - pow(n, 2.0) *( 1 - pow(c1, 2.0));
				
				// Don't refract otherwise, total internal reflection.
				if (c2_inside >= EPSILON) {
					
					double c2 = sqrt(c2_inside);
					refractedRay.dir = (n * ray.dir) + (n * c1 - c2) * 
							normalMultiplier * ray.intersection.normal;
					refractedRay.dir.normalize();

					// refractedRay.origin = refractedRay.origin + 
					// 	0.1 *  refractedRay.dir;

					// Somehow this always happen
					if (refractedRay.dir.dot(ray.dir) < 0) {

						//LOL of course this happen when they are travelling at the negative z direction in the first place
						// std::cout << "Interesting\n";
					}
							

					Color res = shadeRay(
							refractedRay,
							scene,
							light_list,
							depth - 1,
							n2
					);

					// col = col + transparency * res;
					col = col + transmittance  * res;
				}

			}
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

			for (int sampleNum = 0; sampleNum < samples.size(); sampleNum++) {

				traverseScene(scene, *samples[sampleNum], 
						!TRANSPARENT_OBJECTS_CAST_SHADOWS
				); 

				if (samples[sampleNum] -> intersection.none) {
					// The light was unobstructed
					computeShading(ray, light_list[i]);
					hits++;
					colorSum = colorSum + ray.col;
				}

				delete samples[sampleNum];
			}



			if (hits == 0) {
				// Just use light ambient.
				col = col + ambientLightColor;
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

	int numCompleted = 0;

	// Compute the ambient light.
	ambientLightColor[0] = 0.0;
	ambientLightColor[1] = 0.0;
	ambientLightColor[2] = 0.0;

	for (int i = 0; i < light_list.size(); i++) {
		ambientLightColor = ambientLightColor + light_list[i] -> get_ambient();
	}

	ambientLightColor = (1.0 / light_list.size()) * ambientLightColor;

	//multithreading
	#pragma omp parallel for
	// Construct a ray for each pixel.
	for (int i = 0; i < image.height; i++) {

		if (i % (image.height / 10) == 0) {
			std::cout << ++numCompleted << " done\n";

			// int k = 0;

			// while (k < 10 * (double(i) / image.height)) {
			// 	std::cout << '=';
			// 	k++;
			// }

			// while (k < 10) {
			// 	std::cout << '-';
			// 	k++;
			// }

			// std::cout << "]\n";
		}

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

			Color colCentre = shadeRay(
					ray, 
					scene, 
					light_list, 
					RAY_TRACE_DEPTH, 
					AIR_REFRACTIVE
			); 
			
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

						Color aa_res = shadeRay(
								ray2,
								scene,
								light_list, 
								RAY_TRACE_DEPTH,
								AIR_REFRACTIVE
						);

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
				for(int i =0; i < 50; i++){
					//randomly pick points within the radius of the aperture 
					float temp1 =  APERTURE *((float)rand()/(float)(RAND_MAX));
					float temp2 =  APERTURE *((float)rand()/(float)(RAND_MAX));
					Vector3D tempVec (temp1, temp2, 0);
					// Vector3D tempVec (std::fmod(rand()/50, APERTURE), std::fmod(rand()/50, APERTURE), 0);
					Point3D randPoint = ray.origin + viewToWorld * tempVec;
					
					Ray3D secondaryRay;
					secondaryRay.origin = randPoint;
					secondaryRay.dir = focalPoint - randPoint;
					secondaryRay.dir.normalize();
					secondaryRay.intersection.t_value = DBL_MAX;
					// std::cout<< ray.origin << "\n";
					// std::cout << randPoint << "\n";
					// std::cout<<ray.dir << " , " << secondaryRay.dir<< "\n";

					Color dof_res = shadeRay(
							secondaryRay,
							scene,
							light_list,
							RAY_TRACE_DEPTH,
							AIR_REFRACTIVE
					);
					colCentre = colCentre + dof_res;

				}

				colCentre = 0.02 * colCentre; // Divide by 25 points.
				colCentre.clamp();

			}

			if(MOTION_BLUR_ENABLE) {
				float time = 0;

				for (int i = 0 ; i < 100 ; ++i){
					float time = ((float)rand()/(float)(RAND_MAX));   
					
					for ( int j = 0; j< scene.size(); j++){
						// std::cout<< time << "\n";
						Vector3D trans(scene[j]->velocity * time, scene[j]->velocity * time, scene[j]->velocity * time);
						// std::cout << trans << "\n";
						scene[j]->translate(trans);
					}

					Color MB_res = shadeRay(ray,
							scene,
							light_list, 
							RAY_TRACE_DEPTH,
							AIR_REFRACTIVE
					);
				
					for ( int j = 0; j< scene.size(); j++){
						// std::cout<< time << "\n";
						Vector3D trans(- 1.0 * scene[j]->velocity * time, - 1.0 *scene[j]->velocity * time, - 1.0 *scene[j]->velocity * time);
						// std::cout << trans << "\n";
						scene[j]->translate(trans);
					}

					colCentre = colCentre + MB_res; 
					
				}

				colCentre = 0.01 * colCentre; // Divide by 25 points.
				colCentre.clamp();

			}

			image.setColorAtPixel(i, j, colCentre);			
		}
	}
}

