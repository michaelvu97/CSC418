/***********************************************************
	
	Starter code for Assignment 3

***********************************************************/

#include <cstdlib>
#include "raytracer.h"

int main(int argc, char* argv[])
{
	// Build your scene and setup your camera here, by calling 
	// functions from Raytracer.  The code here sets up an example
	// scene and renders it from two different view points, DO NOT
	// change this if you're just implementing part one of the 
	// assignment.  
	Raytracer raytracer;
	LightList light_list;
	Scene scene;   

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

	Material mirror(Color(0.0, 0.0, 0.0), Color(0.2, 0.2, 0.2), 
		Color(0.95, 1.0, 0.95), 70, MIRROR_GLOSSINESS, MIRROR_REFRACTIVE);

	Material red_trans(Color(0.5, 0.0, 0.0), Color(0.5, 0.0, 0.0), 
		Color(0.0, 0.0, 0.0), 70, JADE_GLOSSINESS, REFRACTIVE);

	Material blue_trans(Color(0.0, 0.0, 0.5), Color(0.0, 0.0, 0.5), 
		Color(0.0, 0.0, 0.0), 70, JADE_GLOSSINESS, REFRACTIVE);

	Material glass(Color(0.0, 0.0, 0.0), Color(0.0, 0.0, 0.0), 
		Color(0.0, 0.0, 0.0), 70, 0.0, REFRACTIVE);
	glass.opacity = 0.2;

	// Defines a point light source.
	// PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.1,0.1,0.1));
	// light_list.push_back(pLight);

	ExtendedPointLight* ePLight = new ExtendedPointLight(Point3D(20, 20, 20), 
			Color(0.9, 0.9, 0.9), 5);
	light_list.push_back(ePLight);	

	// ExtendedPointLight* ePLight2 = new ExtendedPointLight(Point3D(-7, -7, -6), 
	// 		Color(0.2, 0.2, 0.7), 3);
	// light_list.push_back(ePLight2);	
	
	// Add a unit square into the scene with material mat.
	SceneNode* sphere = new SceneNode(new UnitSphere(), &glass, 0.1);
	scene.push_back(sphere);
	SceneNode* plane = new SceneNode(new UnitSquare(), &jade, 0);
	scene.push_back(plane);
	SceneNode* sphere2 = new SceneNode(new UnitSphere(), &gold, 0.1);
	scene.push_back(sphere2);
	SceneNode* sphere3 = new SceneNode(new UnitSphere(), &gold, 0.1);
		scene.push_back(sphere3);


	// SceneNode* plane2 = new SceneNode(new UnitSquare(), &gold);
	// scene.push_back(plane2);
	// SceneNode* plane3 = new SceneNode(new UnitSquare(), &gold);
	// scene.push_back(plane3);
	// SceneNode* sphere2 = new SceneNode(new UnitSphere(), &gold, -1);
	// scene.push_back(sphere2);
	

	// Apply some transformations to the sphere and unit square.
	double factor1[3] = { 1.0, 2.0, 1.0 };
	sphere->translate(Vector3D(0, 0, -3));

	sphere2->translate(Vector3D(1.5, 1.5, -5));
	sphere2->rotate('x', -45);
	sphere2->rotate('z', 45);
	sphere2->scale(Point3D(0, 0, 0), factor1);

	sphere3->translate(Vector3D(-1.5, -1.5, -5));
	sphere3->rotate('x', -45);
	sphere3->rotate('z', 45);
	sphere3->scale(Point3D(0, 0, 0), factor1);

	double factor2[3] = { 6.0, 6.0, 6.0 };
	plane->translate(Vector3D(0, 0, -7));
	plane->rotate('z', 45);
	plane->scale(Point3D(0, 0, 0), factor2);

	// plane2->translate(Vector3D(0, 0, -8));
	// plane2->rotate('x', 45);
	// plane2->scale(Point3D(0, 0, 0), factor2);

	// plane3->translate(Vector3D(0, 0, -7));
	// plane3->rotate('z', 45);
	// plane3->scale(Point3D(0, 0, 0), factor2);

	// sphere2->translate(Vector3D(2.5, 1.5, -5));

	// sphere2->translate(Vector3D(0.0, 0.0, -2.0));
	// sphere2->rotate('x', -45);
	// sphere2->rotate('z', 45);
	// sphere2->scale(Point3D(0, 0, 0), factor1);

	//sphere2->translate(Vector3D(2, 2, -5));

	// Render the scene, feel free to make the image smaller for
	// testing purposes.	
	Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
	Image image1(width, height);
	raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image
	image1.flushPixelBuffer("view1.bmp"); //save rendered image to file

	// Render it from a different point of view.
	Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
	Image image2(width, height);
	raytracer.render(camera2, scene, light_list, image2);
	image2.flushPixelBuffer("view2.bmp");

	// Free memory
	for (size_t i = 0; i < scene.size(); ++i) {
		delete scene[i];
	}

	for (size_t i = 0; i < light_list.size(); ++i) {
		delete light_list[i];
	}

	return 0;
}