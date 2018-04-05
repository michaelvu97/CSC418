/***********************************************************
	
	Starter code for Assignment 3
	
	This file contains the interface and datastructures of the raytracer.  
	Simple traversal and addition code to the datastructures are given to you.

***********************************************************/
#pragma once

#include "util.h"
#include "scene_object.h"
#include "light_source.h"

class Raytracer {
public:
	// Renders 3D scene to an image given camera and lights setup.
	void render(Camera& camera, Scene& scene, LightList& light_list, Image& image);
	
	/* 
	 * Convention:  
	 *    2
	 *	1 4 0 5
	 *    3
	 */
	SceneNode* envMapFaces[6];

	// Face -> u + 256 * v -> Color pointer
	Color** envMapData[6];

	int envMapFaceSize = 256;

private:

	// Return the color of the ray after intersection and shading, call 
	// this function recursively for reflection and refraction.  
	Color shadeRay(Ray3D& ray, Scene& scene, LightList& light_list, int depth, 
			float index);

	// Traversal code for the scene, the ray is transformed into 
	// the object space of each node where intersection is performed.
	void traverseScene(Scene& scene, Ray3D& ray, bool ignoreTransparent, double limit);
	void traverseScene(Scene& scene, Ray3D& ray, bool ignoreTransparent);

	// After intersection, calculate the color of the ray by shading it
	// with all light sources in the scene.
	void computeShading(Ray3D& ray, LightList& light_list);
	void computeShading(Ray3D& ray, LightSource* light_list);

	// Precompute the modelToWorld and worldToModel transformations for each
    // object in the scene.
	void computeTransforms(Scene& scene);

	

};
