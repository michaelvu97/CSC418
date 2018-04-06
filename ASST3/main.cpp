#include <cstdlib>
#include "raytracer.h"
#include "NormalMap.h"
#include <time.h>
#include "util.h"

/*************************** GLOBAL PARAMETERS ********************************/

// Enable soft shadows?
const bool SOFT_SHADOWS_ENABLE = false;

// Enable AA?
const bool ANTI_ALIASING_ENABLED = false;

// Enable depth of field?
const bool DOF_ENABLE = false;

// Should transparent objects cast shadows from light sources?
const bool TRANSPARENT_OBJECTS_CAST_SHADOWS = false;

// Use the environment map?
const bool USE_ENV_MAP = true;

/*
 * Should the ambient light color be calculated from the given lights?
 * Set to false for a much better/photorealistic look.
 */
const bool USE_LIGHT_AMBIENT = false;

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
const int RAY_TRACE_DEPTH = 3;

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
const double BLOO_GLOSSINESS 	= 0.3;

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
const double FOCAL_LENGTH 	= 6.0;
const double APERTURE 		= 0.1;
const int 	 DOF_NUM_RAYS	= 30; // 30 is good

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
	LightList light_list, materialDemoLightList;
	Scene scene1, sceneMirrorRoom, sceneMaterialDemo;   

	int width = 320;
	int height = 240;

	if (argc == 3) {
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	}

	if (USE_ENV_MAP) {
		unsigned char* cubeMap = readBMP("cubemap.bmp");

		// We are using a static face size of 256.

		unsigned int face_size = 1024;

		unsigned int img_width = 4 * face_size, 
					 img_height = 3 * face_size;

		for (unsigned int faceNum = 0; faceNum < 6; faceNum++) {

			int xOffSet, yOffset;

			switch(faceNum) {
				case 0:
					xOffSet = 2 * face_size;
					yOffset = face_size;
					break; // offset here.
				case 1: 
					xOffSet = 0;
					yOffset = face_size;
					break;
				case 2:
					xOffSet = face_size;
					yOffset = 0;
					break;
				case 3:
					xOffSet= face_size;
					yOffset = 2 * face_size;
					break;
				case 4:
					xOffSet = face_size;
					yOffset = face_size;
					break;
				case 5:
					xOffSet = 3 * face_size;
					yOffset = face_size;
					break;
				default:
					break;
			}

			raytracer.envMapData[faceNum] = (Color**) malloc(
					sizeof(Color*) * face_size * face_size
			);

			for (unsigned int dx = 0; dx < face_size; dx++) {
				for (unsigned int dy = 0; dy < face_size; dy++) {

					int pixelLocation =
						((yOffset + dy) * img_width) + dx + xOffSet;

					raytracer.envMapData[faceNum][dx + face_size * dy] = new Color(
						double(cubeMap[3 * pixelLocation]) / 256, 
						double(cubeMap[3 * pixelLocation + 1]) / 256, 
						double(cubeMap[3 * pixelLocation + 2]) / 256
					);					

				}
			}
		}

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

	Material red(Color(0.2, 0, 0), Color(0.7, 0.2, 0.2), Color(0.8, 0.3, 0.3),
		10, BLOO_GLOSSINESS, JADE_REFRACTIVE);

	Material mirror(Color(0.0, 0.0, 0.0), Color(0.2, 0.2, 0.2), 
		Color(0.97, 0.99, 0.97), 100, MIRROR_GLOSSINESS, MIRROR_REFRACTIVE);

	Material red_trans(Color(1, 0.0, 0.0), Color(1, 0.0, 0.0), 
		Color(0.0, 0.0, 0.0), 70, JADE_GLOSSINESS, REFRACTIVE);

	Material blue_trans(Color(0.0, 0.0, 1), Color(0.0, 0.0, 1), 
		Color(0.0, 0.0, 0.0), 70, JADE_GLOSSINESS, REFRACTIVE);

	Material glass(Color(0.0, 0.0, 0.0), Color(0.0, 0.0, 0.0), 
		Color(0.0, 0.0, 0.0), 70, GLASS_GLOSSINESS, REFRACTIVE);
	glass.opacity = 0.2;

	Material neutral(Color(0.3, 0.3, 0.3), Color(0.6, 0.6, 0.6), 
		Color(0.3, 0.3, 0.3), 40, 0.3, JADE_REFRACTIVE);

	// Defines a point light source.
	// PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.1,0.1,0.1));
	// light_list.push_back(pLight);

	ExtendedPointLight* ePLight = new ExtendedPointLight(Point3D(20, 20, 20), 
			Color(0.0, 0.0, 0.0), Color(1.0, 1.0, 1.0), Color(1.0, 1.0, 1.0), 5);
	light_list.push_back(ePLight);	

	// ExtendedPointLight* ePLight2 = new ExtendedPointLight(Point3D(0.5, 0.5, -1), 
	// 		Color(0.0, 0.0, 0.0), Color(0.2, 0.2, 0.9), Color(0.2, 0.2, 0.9), 5);
	// light_list.push_back(ePLight2);	

	// ExtendedPointLight* ePLight3 = new ExtendedPointLight(Point3D(20, 20, 20), 
	// 		Color(0.0, 0.0, 0.0), Color(1.0, 1.0, 1.0), Color(1.0, 1.0, 1.0), 8);
	// light_list.push_back(ePLight3);

	// ExtendedPointLight* ePLight2 = new ExtendedPointLight(Point3D(-7, -7, -6), 
	// 		Color(0.2, 0.2, 0.7), 3);
	// light_list.push_back(ePLight2);	
	
	// Add a unit square into the scene with material mat.





	
	ISOLATE { // Isolate local variables from other scenes.

		/*
		 * PRETTY SCENE
		 */

		SceneNode* sphere = new SceneNode(new UnitSphere(), &glass);
		scene1.push_back(sphere);

		SceneNode* sphere2 = new SceneNode(new UnitSphere(), &gold);
		scene1.push_back(sphere2);
		SceneNode* sphere3 = new SceneNode(new UnitSphere(), &gold);
		scene1.push_back(sphere3);
		SceneNode* sphere4 = new SceneNode(new UnitSphere(), &mirror);
		scene1.push_back(sphere4);
		SceneNode* sphere5 = new SceneNode(new UnitSphere(), &mirror);
		scene1.push_back(sphere5);

		SceneNode* plane = new SceneNode(new UnitSquare(), &jade);
		scene1.push_back(plane);

		SceneNode* plane2 = new SceneNode(new UnitSquare(), &bloo);
		scene1.push_back(plane2);

		/* 
		 * Normal Mapping
		 */
		sphere -> obj -> normalMap.push_back(new MetallicGrainNormal(500));

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
	
	ISOLATE { // Isolate local variables from other scenes.

		/*
		 * MIRROR ROOM SCENE
		 */

		SceneNode* sphere = new SceneNode(new UnitSphere(), &gold);
		sceneMirrorRoom.push_back(sphere);

		SceneNode* plane = new SceneNode(new UnitSquare(), &mirror);
		sceneMirrorRoom.push_back(plane);
		SceneNode* plane2 = new SceneNode(new UnitSquare(), &mirror);
		sceneMirrorRoom.push_back(plane2);
		SceneNode* plane3 = new SceneNode(new UnitSquare(), &mirror);
		sceneMirrorRoom.push_back(plane3);
		SceneNode* plane4 = new SceneNode(new UnitSquare(), &mirror);
		sceneMirrorRoom.push_back(plane4);

		SceneNode* plane5 = new SceneNode(new UnitSquare(), &mirror);
		sceneMirrorRoom.push_back(plane5);

		SceneNode* plane6 = new SceneNode(new UnitSquare(), &mirror);
		sceneMirrorRoom.push_back(plane6);


		sphere -> obj -> normalMap.push_back(new MetallicGrainNormal(500));

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

	ISOLATE { // Isolate local variables from other scenes.
		/*
		 * Material Demo Scene
		 */

		ExtendedPointLight* behindCameraLight = new ExtendedPointLight(
				Point3D(0.0, 0.0, -1.1), 
				Color(0.1, 0.1, 0.1), 
				Color(0.8, 0.8, 0.81), 
				Color(0.7, 0.7, 0.7), 
				1
		);

		ExtendedPointLight* diffuseAboveLight = new ExtendedPointLight(
				Point3D(0.0, 1.0, -2.0), 
				Color(0.1, 0.1, 0.1), 
				Color(0.3, 0.3, 0.33), 
				Color(0.4, 0.4, 0.4), 
				1.5
		);

		materialDemoLightList.push_back(diffuseAboveLight);
		materialDemoLightList.push_back(behindCameraLight);

		// TODO maybe make these less intense colors.
		SceneNode* ground 		= new SceneNode(new UnitSquare(), &jade);
		SceneNode* leftWall 	= new SceneNode(new UnitSquare(), &neutral);
		SceneNode* rightWall 	= new SceneNode(new UnitSquare(), &neutral);
		SceneNode* ceiling 		= new SceneNode(new UnitSquare(), &neutral);
		SceneNode* backWall 	= new SceneNode(new UnitSquare(), &neutral);
		SceneNode* behindCamera = new SceneNode(new UnitSquare(), &neutral);

		ground -> obj -> normalMap.push_back(new CorrugatedNormal());

		// sceneMaterialDemo.push_back(ground);
		// sceneMaterialDemo.push_back(leftWall);
		// sceneMaterialDemo.push_back(rightWall);
		// sceneMaterialDemo.push_back(ceiling);
		// sceneMaterialDemo.push_back(backWall);
		// sceneMaterialDemo.push_back(behindCamera);

		double roomScaleFactor[3] = {10, 10, 10};

		// ground -> translate(Vector3D(0, -1, -3));
		ground -> translate(Vector3D(0, -3, -3));
		ground -> rotate('x', -90);
		ground -> rotate('z', 30);

		backWall -> translate(Vector3D(0, 4, -8));

		behindCamera -> translate(Vector3D(0, 4, 2));
		behindCamera -> rotate('y', 180);

		leftWall -> translate(Vector3D(-5, 4, -3));
		leftWall -> rotate('y', 90);

		rightWall -> translate(Vector3D(5, 4, -3));
		rightWall -> rotate('y', -90);

		ceiling -> translate(Vector3D(0, 9, -3));
		ceiling -> rotate('x', 90);

		ground       -> scale(Point3D(0, 0, 0), roomScaleFactor);
		leftWall     -> scale(Point3D(0, 0, 0), roomScaleFactor);
		rightWall    -> scale(Point3D(0, 0, 0), roomScaleFactor);
		ceiling      -> scale(Point3D(0, 0, 0), roomScaleFactor);
		backWall	 -> scale(Point3D(0, 0, 0), roomScaleFactor);
		behindCamera -> scale(Point3D(0, 0, 0), roomScaleFactor);


		// SceneNode* ball_1 = new SceneNode(new UnitSphere(), &gold);
		// SceneNode* ball_2 = new SceneNode(new UnitSphere(), &jade);
		// SceneNode* ball_3 = new SceneNode(new UnitSphere(), &gold);
		// SceneNode* ball_4 = new SceneNode(new UnitSphere(), &mirror);

		SceneNode* ball_1 = new SceneNode(new UnitSphere(), &mirror);
		SceneNode* ball_2 = new SceneNode(new UnitSphere(), &mirror);
		SceneNode* ball_3 = new SceneNode(new UnitSphere(), &mirror);
		SceneNode* ball_4 = new SceneNode(new UnitSphere(), &mirror);

		SceneNode* ball_5 = new SceneNode(new UnitSphere(), &gold);

		sceneMaterialDemo.push_back(ball_1);
		sceneMaterialDemo.push_back(ball_2);
		sceneMaterialDemo.push_back(ball_3);
		sceneMaterialDemo.push_back(ball_4);
		sceneMaterialDemo.push_back(ball_5);

		// ball_1 -> obj -> normalMap.push_back(new MetallicGrainNormal(500));
		ball_1 -> obj -> normalMap.push_back(new MetallicGrainNormal(2000));
		ball_2 -> obj -> normalMap.push_back(new CorrugatedNormal());
		ball_3 -> obj -> normalMap.push_back(new NoiseyNormal(0.6));
		ball_4 -> obj -> normalMap.push_back(new PolynomialNoiseNormal(15));

		double bally = 0;
		double ballz = -5;

		ball_1 -> translate(Vector3D(-3, bally, ballz));
		ball_2 -> translate(Vector3D(-1, bally, ballz));
		ball_3 -> translate(Vector3D(1, bally, ballz));
		ball_4 -> translate(Vector3D(3, bally, ballz));

		double smolFactor[3] = {15, 15, 1};
		ball_5 -> scale(Point3D(0, 0, 0), smolFactor);

		ball_5 -> translate(Vector3D(0, 0, 4));


	}

	// Render the scene, feel free to make the image smaller for
	// testing purposes.	
	Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 90.0);
	Image image1(width, height);
	// raytracer.render(camera1, sceneMaterialDemo, materialDemoLightList, image1); //render 3D scene to image
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

	for (size_t i = 0; i < sceneMaterialDemo.size(); ++i) {
		delete sceneMaterialDemo[i];
	}

	for (size_t i = 0; i < light_list.size(); ++i) {
		delete light_list[i];
	}

	return 0;
}