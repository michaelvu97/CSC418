#include <cstdlib>
#include "raytracer.h"
#include "NormalMap.h"
#include <time.h>
#include "util.h"

/*************************** GLOBAL PARAMETERS ********************************/

// Number of gloss shells to produce. 0 disables glossy reflections.
const int DEFAULT_GLOSS_SHELLS = 0;

// Enable soft shadows?
const bool SOFT_SHADOWS_ENABLE = false;

// Enable AA?
const bool ANTI_ALIASING_ENABLED = false;

// Enable depth of field?
const bool DOF_ENABLE = false;

// Should transparent objects cast shadows from light sources?
const bool TRANSPARENT_OBJECTS_CAST_SHADOWS = false;

/*
 * Gloss Parameters.
 *
 * 		Regularizer: 
 *				Constant multiplier of glossy intensity.
 *
 *		Exponent Regularizer: 
 *				Exponentiation of gloss angle intensity multiplier.
 *
 */
const double GLOSS_REGULARIZER 			= 3;
const double GLOSS_EXPONENT_REGULARIZER = 7;

/*
 * Soft Shadow Parameters.
 *
 * 		Delta:
 * 				How much should the extended light volume be divided for soft
 *				shadow sampling? Number increases samples cubically. 4 is a 
 *				reasonable value for this parameter.
 *				
 */
const double SOFT_SHADOWS_DELTA = 4;

/*
 * AA Parameters.
 *
 *		Delta:
 *				What distance should the anti-aliasing samples deviate from the
 *				centre of the pixel?
 *
 */
const double ANTI_ALIASING_DELTA = 0.3;

/*
 * General Ray Tracing Parameters.
 *		
 *		Depth:
 *				How many bounces should each ray make before it is terminated?
 *
 */
const int RAY_TRACE_DEPTH = 5;

/*
 * Material Glossiness Parameters.
 *
 *		The higher the value, the more "smooth" the object is. 1.0 is a perfect
 *		mirror, 0.0 is a completely matte object.
 *
 */
const double GOLD_GLOSSINESS 	= 0.6;
const double JADE_GLOSSINESS 	= 0.8;
const double MIRROR_GLOSSINESS 	= 1.0;
const double GLASS_GLOSSINESS 	= 1.0;
const double BLOO_GLOSSINESS 	= 0.2;

/*
 * Depth of Field Parameters.
 *
 * 		Focal Length:
 *				Distance from the camera where the focal point will lie. The
 *				focal plane is actually spherical since it is measured from the 
 *				camera origin.
 *		
 *		Aperture:
 *				Size of the simulated camera aperture. 0 is a pinhole camera,
 *				and as the number increase, makes out-of-focus objects
 *				blurrier.
 *
 *		Num Rays:
 *				Number of samples to estimate the depth of field effect. The
 *				higher the number, the less noisey the effect is and the better
 *				it approximates real light, but the computation takes longer.
 *			
 */
const double FOCAL_LENGTH 	= 4.0;
const double APERTURE 		= 0.1;
const int 	 DOF_NUM_RAYS	= 30;

/*
 * Material Refractive Constants.
 */
const double GOLD_REFRACTIVE = EPSILON;
const double JADE_REFRACTIVE = EPSILON;
const double MIRROR_REFRACTIVE = EPSILON;
const double REFRACTIVE = 3.0;
const double AIR_REFRACTIVE = 1.0;

/******************************************************************************/


int main(int argc, char* argv[])
{
	// Build your scene and setup your camera here, by calling 
	// functions from Raytracer.  The code here sets up an example
	// scene and renders it from two different view points, DO NOT
	// change this if you're just implementing part one of the 
	// assignment.  
	Raytracer raytracer;
	LightList light_list;
	Scene scene1, sceneMirrorRoom;   

	int width = 320;
	int height = 240;

	if (argc == 3) {
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	}

	srand(time(NULL));
	
	// Define materials for shading.
	Material gold(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
		Color(0.628281, 0.555802, 0.366065),
		51.2, GOLD_GLOSSINESS, GOLD_REFRACTIVE);
	Material jade(Color(0, 0, 0), Color(0.54,0.89,0.63),
		Color(0.316228,0.316228,0.316228),
		12.8, JADE_GLOSSINESS, JADE_REFRACTIVE);

	Material bloo(Color(0, 0, 0.2), Color(0.2, 0.2, 0.7), Color(0.3, 0.3, 0.8),
		10, BLOO_GLOSSINESS, JADE_REFRACTIVE);

	Material mirror(Color(0.0, 0.0, 0.0), Color(0.2, 0.2, 0.2), 
		Color(0.98, 1.0, 0.98), 100, MIRROR_GLOSSINESS, MIRROR_REFRACTIVE);

	Material red_trans(Color(1, 0.0, 0.0), Color(1, 0.0, 0.0), 
		Color(0.0, 0.0, 0.0), 70, JADE_GLOSSINESS, REFRACTIVE);

	Material blue_trans(Color(0.0, 0.0, 1), Color(0.0, 0.0, 1), 
		Color(0.0, 0.0, 0.0), 70, JADE_GLOSSINESS, REFRACTIVE);

	Material glass(Color(0.0, 0.0, 0.0), Color(0.0, 0.0, 0.0), 
		Color(0.0, 0.0, 0.0), 70, GLASS_GLOSSINESS, REFRACTIVE);
	glass.opacity = 0.2;

	// Defines a point light source.
	// PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.1,0.1,0.1));
	// light_list.push_back(pLight);

	ExtendedPointLight* ePLight = new ExtendedPointLight(Point3D(20, 20, 20), 
			Color(0.0, 0.0, 0.0), Color(1.0, 1.0, 1.0), Color(1.0, 1.0, 1.0), 5);
	light_list.push_back(ePLight);	

	ExtendedPointLight* ePLight2 = new ExtendedPointLight(Point3D(0.5, 0.5, -1), 
			Color(0.0, 0.0, 0.0), Color(0.2, 0.2, 0.9), Color(0.2, 0.2, 0.9), 5);
	light_list.push_back(ePLight2);	

	// ExtendedPointLight* ePLight3 = new ExtendedPointLight(Point3D(20, 20, 20), 
	// 		Color(0.0, 0.0, 0.0), Color(1.0, 1.0, 1.0), Color(1.0, 1.0, 1.0), 8);
	// light_list.push_back(ePLight3);

	// ExtendedPointLight* ePLight2 = new ExtendedPointLight(Point3D(-7, -7, -6), 
	// 		Color(0.2, 0.2, 0.7), 3);
	// light_list.push_back(ePLight2);	
	
	// Add a unit square into the scene with material mat.





	/*
	 * PRETTY SCENE
	 */
	ISOLATE { // Isolate local variables from other scenes.

		SceneNode* sphere = new SceneNode(new UnitSphere(), &glass, 0.1);
		scene1.push_back(sphere);

		SceneNode* sphere2 = new SceneNode(new UnitSphere(), &gold, 0.1);
		scene1.push_back(sphere2);
		SceneNode* sphere3 = new SceneNode(new UnitSphere(), &gold, 0.1);
		scene1.push_back(sphere3);
		SceneNode* sphere4 = new SceneNode(new UnitSphere(), &mirror, 0.1);
		scene1.push_back(sphere4);
		SceneNode* sphere5 = new SceneNode(new UnitSphere(), &mirror, 0.1);
		scene1.push_back(sphere5);

		SceneNode* plane = new SceneNode(new UnitSquare(), &jade, 0);
		scene1.push_back(plane);

		SceneNode* plane2 = new SceneNode(new UnitSquare(), &bloo, 0);
		scene1.push_back(plane2);

		/* 
		 * Normal Mapping
		 */
		plane -> obj -> normalMap.push_back(new MetallicGrainNormal(500));
		// plane2 -> obj -> normalMap.push_back(new PolynomialNoiseNormal(5));
		// sphere2 -> obj -> normalMap.push_back(new MetallicGrainNormal(5000));
		// sphere3 -> obj -> normalMap.push_back(new MetallicGrainNormal(500));
		// sphere2 -> obj -> normalMap.push_back(new NoiseyNormal(0.02));
		// sphere3 -> obj -> normalMap.push_back(new NoiseyNormal(0.02));

		// sphere4 -> obj -> normalMap.push_back(new MetallicGrainNormal(500));
		// sphere4 -> obj -> normalMap.push_back(new NoiseyNormal(0.02));

		double factor1[3] = { 1.0, 2.0, 1.0 };
		double factor2[3] = { 6.0, 6.0, 6.0 };
		double factor3[3] = { 2.0, 2.0, 2.0 };

		sphere->translate(Vector3D(0, 0, -3));
		sphere2->translate(Vector3D(-1.0, -1.0, -5));
		sphere3->translate(Vector3D(-1.0, 1.0, -5));
		sphere4->translate(Vector3D(1.0, -1.0, -5));
		sphere5->translate(Vector3D(1.0, 1.0, -5));

		plane->translate(Vector3D(0, 0, -7));
		plane->rotate('z', 45);
		plane->scale(Point3D(0, 0, 0), factor2);

		plane2 -> translate(Vector3D(-2, -2, -7 + 3));
		plane2 -> rotate('y', 90);
		plane2 -> rotate('x', -45);
		plane2 -> scale(Point3D(0,0,0), factor2);
	}
	/*
	 * END PRETTY SCENE
	 */

	/*
	 * MIRROR ROOM SCENE
	 */
	ISOLATE { // Isolate local variables from other scenes.
		SceneNode* sphere = new SceneNode(new UnitSphere(), &glass, 0.1);
		sceneMirrorRoom.push_back(sphere);

		SceneNode* plane = new SceneNode(new UnitSquare(), &mirror, 0);
		sceneMirrorRoom.push_back(plane);
		SceneNode* plane2 = new SceneNode(new UnitSquare(), &mirror, 0);
		sceneMirrorRoom.push_back(plane2);
		SceneNode* plane3 = new SceneNode(new UnitSquare(), &mirror, 0);
		sceneMirrorRoom.push_back(plane3);
		SceneNode* plane4 = new SceneNode(new UnitSquare(), &mirror, 0);
		sceneMirrorRoom.push_back(plane4);

		SceneNode* plane5 = new SceneNode(new UnitSquare(), &mirror, 0);
		sceneMirrorRoom.push_back(plane5);

		SceneNode* plane6 = new SceneNode(new UnitSquare(), &mirror, 0);
		sceneMirrorRoom.push_back(plane6);



		double factor4[3] = {6, 6, 6};
		double factor5[3] = {10, 6, 1};
		plane -> translate(Vector3D(0, 0, -8));
		plane -> scale (Point3D(0, 0, 0), factor4);

		sphere -> translate(Vector3D(0, 0, -3));

		plane2 -> translate(Vector3D(0, 0, 2));
		plane2 -> scale(Point3D(0,0,0), factor4);

		plane3 -> translate(Vector3D(-3, 0, -5));
		plane3 -> rotate('y', 90);
		plane3 -> scale(Point3D(0,0,0), factor5);

		plane4 -> translate(Vector3D(3, 0, -5));
		plane4 -> rotate('y', -90);
		plane4 -> scale(Point3D(0,0,0), factor5);
		
		plane5 -> translate(Vector3D(0, 3, -5));
		plane5 -> rotate('x', 90);
		plane5 -> scale(Point3D(0,0,0), factor5);

		plane6 -> translate(Vector3D(0, -3, -5));
		plane6 -> rotate('x', -90);
		plane6 -> scale(Point3D(0,0,0), factor5);
	}
	/*
	 * END MIRROR ROOM SCENE
	 */

	// Render the scene, feel free to make the image smaller for
	// testing purposes.	
	Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
	Image image1(width, height);
	raytracer.render(camera1, scene1, light_list, image1); //render 3D scene to image
	image1.flushPixelBuffer("view1.bmp"); //save rendered image to file

	// Render it from a different point of view.
	Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
	Image image2(width, height);
	raytracer.render(camera2, scene1, light_list, image2);
	image2.flushPixelBuffer("view2.bmp");

	// Free memory
	for (size_t i = 0; i < scene1.size(); ++i) {
		delete scene1[i];
	}

	for (size_t i = 0; i < sceneMirrorRoom.size(); ++i) {
		delete sceneMirrorRoom[i];
	}

	for (size_t i = 0; i < light_list.size(); ++i) {
		delete light_list[i];
	}

	return 0;
}