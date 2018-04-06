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



void Raytracer::traverseScene(Scene& scene, Ray3D& ray, 
			bool ignoreTransparent) {
	this -> traverseScene(scene, ray, ignoreTransparent, DBL_MAX);
}

void Raytracer::traverseScene(Scene& scene, Ray3D& ray, bool ignoreTransparent, 
		double limit){

	for (size_t i = 0; i < scene.size(); ++i) {
		SceneNode* node = scene[i];

		if (ignoreTransparent && node -> mat -> opacity < 1.0 - EPSILON)
			continue;

		if (node->obj->intersect(ray, node->worldToModel, node->modelToWorld, limit)) {
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
		light->shade(ray);        
	}

}

Color Raytracer::shadeRay(Ray3D& ray, Scene& scene, LightList& light_list, 
		int depth, float index) {

	Color col(0.0, 0.0, 0.0); 

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

		}

		double transmittance = 1.0 - reflectance; 

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
			double g = 1.0 - ray.intersection.mat -> glossiness;

			Vector3D deltaYVec = GetArbitraryTangentFromNorm(ray.dir);
			Vector3D deltaXVec = ray.dir.cross(deltaYVec);

			deltaYVec.normalize();
			deltaXVec.normalize();

			Color specularSum(0.0, 0.0, 0.0);
			Color diffuseSum(0.0, 0.0, 0.0);

			// Must be greater than 0, more shells is more gloss rays.
			int shells = DEFAULT_GLOSS_SHELLS;

			/*
			 * Calculate glossy reflections.
			 */
			if (shells > 0 && g > EPSILON) {

				int ignoredRays = 0;

				for (double dx = -1; dx <= 1 ; dx += 1.0 / shells) {
					for (double dy = -1; dy <= 1; dy +=  1.0 / shells) {

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
								// depth / 2 > 0 ? depth / 2 : 0,
								0,
								index
						);

						/*
						 * In the interest of computation time and looking good,
						 * the multiplier for the color has been linearized.
						 */
						diffuseSum = diffuseSum + 
								pow(
										tempRay.dir.dot(rayReflection.dir), 
										(1.0 - g) * GLOSS_EXPONENT_REGULARIZER
								) * c;

					}
				}

				// Average
				diffuseSum = ( g * GLOSS_REGULARIZER / 
						(pow(1.0 + 2.0 * shells, 2) - ignoredRays) ) * diffuseSum;

				diffuseSum.clamp();

			}
			// Add the original ray
			specularSum = specularSum + (1.0 - g) * (1.0 - g) * shadeRay(rayReflection, scene, light_list,
				depth - 1, index);	

			

			specularSum.clamp();

			double opacity = ray.intersection.mat -> opacity;
			double transparency = 1.0 - opacity;

			col = col + reflectance * (
					Phong(
						rayReflection.dir,
						ray.intersection.normal,
						ray.dir,
						ray.intersection.mat,
						ambientLightColor,
						diffuseSum,

						// This is experimental right now. Normally use diffuse
						// instead of the 0 0 0 color above.
						specularSum
					)
			);

			col.clamp();
			
			/*
			 * Handle refraction.
			 */
			if (transparency > EPSILON) {

				/*
				 * Generate refracted ray.
				 */
				Ray3D refractedRay;
				refractedRay.origin = ray.intersection.point;

				float n1 = index;

				float n2 = insideObject ? 
						AIR_REFRACTIVE : 
						ray.intersection.mat->refraction_index;

				float n = n1 / n2;

				double normalMultiplier = insideObject ? -1.0 : 1.0;

				double c1 = -1.0 * normalMultiplier * 
						(ray.intersection.normal.dot(ray.dir));

				double c2_inside = 1.0 - pow(n, 2.0) *( 1 - pow(c1, 2.0));
				
				// Check for total internal reflection.
				if (c2_inside >= EPSILON) {
					

					double c2 = sqrt(c2_inside);

					refractedRay.dir = 
							(n * ray.dir) + 
							(n * c1 - c2) * normalMultiplier * 
							ray.intersection.normal;

					refractedRay.dir.normalize();


					Color res = shadeRay(
							refractedRay,
							scene,
							light_list,
							depth - 1,
							n2
					);

					col = col + transmittance  * res;
				}

			}
		}

		/*
		 * Calculate the lighting at this point.
		 */
		for (int i = 0; i < light_list.size(); i++) {

			Ray3D newLightRay;
			newLightRay.origin = ray.intersection.point;

			newLightRay.dir = 
					(light_list[i] -> get_position()) - 
					newLightRay.origin;

			double dist = newLightRay.dir.length();
			newLightRay.dir.normalize();

			std::vector<Ray3D*> samples = 
					light_list[i] -> generateSamples(newLightRay);

			Color colorSum(0, 0, 0);

			
			int hits = 0;

			for (int sampleNum = 0; sampleNum < samples.size(); sampleNum++) {

				traverseScene(scene, *samples[sampleNum], 
						!TRANSPARENT_OBJECTS_CAST_SHADOWS,
						dist
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

		
	} else if (USE_ENV_MAP) {
		/*
		 * Calculate the environment map intersection.
		 */

		for (size_t i = 0; i < 6; ++i) {
			SceneNode* node = this -> envMapFaces[i];

			node->obj->intersect(ray, node->worldToModel, node->modelToWorld, 
					DBL_MAX
			);

		}

		// Determine which face was hit.
		if (ray.intersection.none) {
			std::cout << "Missed environment map, the containing box may not be\
			 sealed\n";
		}

		Point3D p = ray.intersection.point;

		int face = -1;

		double u = 0.0, v = 0.0;

		double envMapEpsilon = 0.01;

		if (p[0] > 500.0 - envMapEpsilon && p[0] < 500.0 + envMapEpsilon) {
			face = 0;
			u = (p[1] + 500) / 1000.0;
			v = (p[2] + 500) / 1000.0;
		} else if (p[0] > -500 - envMapEpsilon && p[0] < -500 + envMapEpsilon) {
			face = 1;
			u = (500 - p[1]) / 1000.0;
			v = (p[2] + 500) / 1000.0;
		} else if (p[1] > 500 - envMapEpsilon && p[1] < 500 + envMapEpsilon) {
			face = 5;
			u = (500 - p[0]) / 1000.0;
			v = (p[2] + 500) / 1000.0;
		} else if (p[1] > -500 - envMapEpsilon && p[1] < -500 + envMapEpsilon) {
			face = 4;
			u = (p[0] + 500) / 1000.0;
			v = (p[2] + 500) / 1000.0;
		} else if (p[2] > 500 - envMapEpsilon && p[2] < 500 + envMapEpsilon) {
			face = 3;
			u = (p[0] + 500) / 1000.0;
			v = (p[1] + 500) / 1000.0;
		} else if (p[2] > -500 - envMapEpsilon && p[2] < -500 + envMapEpsilon) {
			face = 2;	
			u = (p[0] + 500) / 1000.0;
			v = (500 - p[1]) / 1000.0;
		} else {
			std::cout << "failure: " << p << "\n";
		}

		u *= this -> envMapFaceSize;
		v *= this -> envMapFaceSize;

		int pixelU = int (u);
		int pixelV = int (v);

		/*
		 * UV clamping.
		 */
		if (pixelU < 0)
			pixelU = 0;

		if (pixelU > this -> envMapFaceSize - 1)
			pixelU = this -> envMapFaceSize - 1;

		if (pixelV < 0)
			pixelV = 0;

		if (pixelV > this -> envMapFaceSize - 1)
			pixelV = this -> envMapFaceSize - 1;

		if (face != -1) {
			col = col + 
					*(this -> envMapData[face][
							int(pixelU) + this ->envMapFaceSize * int(pixelV)
					]);

		}


	}

	col.clamp();
	return col; 

}	

void Raytracer::render(Camera& camera, Scene& scene, LightList& light_list, 
		Image& image) {

	computeTransforms(scene);

	Matrix4x4 viewToWorld;
	double factor = (double(image.height)/2)/tan(camera.fov*M_PI/360.0);

	viewToWorld = camera.initInvViewMatrix();

	if (USE_ENV_MAP) {
		/* 
		 * Set up the environment map walls at "infinity" (actually just 1000)
		 */

		Material* nullMaterial = new Material(Color(0, 0, 0), Color(0, 0, 0), 
				Color(0, 0, 0), 1, 1, 0);

		for (int i = 0; i < 6; i++) {
			this -> envMapFaces[i] = new SceneNode(
					new UnitSquare(), 
					nullMaterial
			);	
		}

		// Ground
		this -> envMapFaces[3] -> translate(Vector3D(0, -500, 0));
		this -> envMapFaces[3] -> rotate('x', -90);

		this -> envMapFaces[4] -> translate(Vector3D(0, 0, -500));

		this -> envMapFaces[5] -> translate(Vector3D(0, 0, 500));
		this -> envMapFaces[5] -> rotate('y', 180);

		this -> envMapFaces[1] -> translate(Vector3D(-500, 0, 0));
		this -> envMapFaces[1] -> rotate('y', 90);

		this -> envMapFaces[0] -> translate(Vector3D(500, 0, 0));
		this -> envMapFaces[0] -> rotate('y', -90);

		this -> envMapFaces[2] -> translate(Vector3D(0, 500, 0));
		this -> envMapFaces[2] -> rotate('x', 90);

		double envMapScaleFactor[3] = {1000.0, 1000.0, 1000.0};

		this -> envMapFaces[3] -> scale(Point3D(0, 0, 0), envMapScaleFactor);
		this -> envMapFaces[1] -> scale(Point3D(0, 0, 0), envMapScaleFactor);
		this -> envMapFaces[0] -> scale(Point3D(0, 0, 0), envMapScaleFactor);
		this -> envMapFaces[2] -> scale(Point3D(0, 0, 0), envMapScaleFactor);
		this -> envMapFaces[4] -> scale(Point3D(0, 0, 0), envMapScaleFactor);
		this -> envMapFaces[5] -> scale(Point3D(0, 0, 0), envMapScaleFactor);
		

		/*
		 * Finalize the transforms for the faces.
		 */
		for (int i = 0; i < 6; i++) {

			this -> envMapFaces[i] -> modelToWorld = 
					this -> envMapFaces[i] -> trans;

			this -> envMapFaces[i] -> worldToModel = 
					this -> envMapFaces[i] -> invtrans;

		}

	}

	// Keep track of the iterations.
	int numCompleted = 0;

	// Compute the ambient light.
	ambientLightColor[0] = 0.0;
	ambientLightColor[1] = 0.0;
	ambientLightColor[2] = 0.0;

	if (USE_LIGHT_AMBIENT) {
		/*
		 * Calculate the ambient lighting average of the light sources.
		 */

		for (int i = 0; i < light_list.size(); i++) {
			ambientLightColor = 
					ambientLightColor + 
					light_list[i] -> get_ambient();
		}

		ambientLightColor = (1.0 / light_list.size()) * ambientLightColor;
	}

	std::cout << "--------------\n";

	// Multithreading
	#pragma omp parallel for

	// Construct a ray for each pixel.
	for (int i = 0; i < image.height; i++) {

		if (i % (image.height / 10) == 0) {
			/*
			 * Just print something to the console to estimate how long each
			 * decade takes.
			 *
			 * There's synchronization errors here because of multithreading
			 * but whatever.
			 */

			std::cout << numCompleted++ << " started\n";

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

				Color antiAliasingColor(0, 0, 0);

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

					 	antiAliasingColor = antiAliasingColor + aa_res; 

					}
				}


				// Divide by 5 points.
				antiAliasingColor = 0.2 * antiAliasingColor; 
				antiAliasingColor.clamp();

				colCentre = colCentre + antiAliasingColor;



			}

			if(DOF_ENABLE) {

				Color dofCol(0, 0, 0);

				// Generate focal point for different ray dir
				Point3D focalPoint = ray.origin + FOCAL_LENGTH * ray.dir;

				for(int i =0; i < DOF_NUM_RAYS; i++){

					// Randomly pick points within the radius of the aperture 
					float temp1 =  APERTURE *((float)rand()/(float)(RAND_MAX));
					float temp2 =  APERTURE *((float)rand()/(float)(RAND_MAX));

					Vector3D tempVec (temp1, temp2, 0);

					Point3D randPoint = ray.origin + viewToWorld * tempVec;
					
					Ray3D secondaryRay;
					secondaryRay.origin = randPoint;
					secondaryRay.dir = focalPoint - randPoint;
					secondaryRay.dir.normalize();
					secondaryRay.intersection.t_value = DBL_MAX;

					Color dof_res = shadeRay(
							secondaryRay,
							scene,
							light_list,
							RAY_TRACE_DEPTH,
							AIR_REFRACTIVE
					);

					dofCol = dofCol + dof_res;

				}

				// Divide by 25 points.
				dofCol = (1.0 / DOF_NUM_RAYS) * dofCol; 

				dofCol.clamp();

				colCentre = colCentre + dofCol;

			}

			image.setColorAtPixel(i, j, colCentre);			

		}

	}

}
