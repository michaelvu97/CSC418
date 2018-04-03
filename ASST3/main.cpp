/***********************************************************
	
	Starter code for Assignment 3

***********************************************************/

#include <cstdlib>
#include "raytracer.h"
#include "NormalMap.h"

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
	if (true) { // Isolate local variables from other scenes.
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
		sphere5 -> obj -> normalMap.push_back(new RadialCorrugatedNormal(0, 0));
		sphere5 -> obj -> normalMap.push_back(new NoiseyNormal(0.02));

		SceneNode* plane = new SceneNode(new UnitSquare(), &jade, 0);
		scene1.push_back(plane);
		// plane -> obj -> normalMap = new CorrugatedNormal();

		SceneNode* plane2 = new SceneNode(new UnitSquare(), &bloo, 0);
		scene1.push_back(plane2);
		// plane2 -> obj -> normalMap = new RadialCorrugatedNormal(0, 0);

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
	if (true) { // Isolate local variables from other scenes.
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