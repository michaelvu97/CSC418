/***********************************************************

	Starter code for Assignment 3
	
	Utility functions and structures (based on code from CGL, University of Waterloo), 
	modify this file as you see fit.

***********************************************************/
#pragma once

#include <iostream>
#include <cmath>
#include "bmp_io.h"
#include <math.h>

#ifndef M_PI
#define M_PI	3.14159265358979323846
#endif

#ifndef GLOBAL_RENDERING_PREFS
#define GLOBAL_RENDERING_PREFS

#define DEFAULT_GLOSS_SHELLS 2 // Set to 0 to disable gloss.
#define GLOSS_REGULARIZER 3
#define GLOSS_EXPONENT_REGULARIZER 7

#define SOFT_SHADOWS_ENABLE 1
#define SOFT_SHADOWS_DELTA 4 // 4 is a good value

#define ANTI_ALIASING_ENABLED 1
#define ANTI_ALIASING_DELTA 0.3

#define RAY_TRACE_DEPTH 3

// [0,1], 1 means perfect mirror, 0 means very diffuse.
#define GOLD_GLOSSINESS 0.6
#define JADE_GLOSSINESS 0.8
#define MIRROR_GLOSSINESS 1.0
#define GLASS_GLOSSINESS 1.0
#define BLOO_GLOSSINESS 0.2

#define DOF_ENABLE 0
#define FOCAL_LENGTH 5.0
#define APERTURE 1.1

#ifndef EPSILON
#define EPSILON 0.001
#endif

#define TRANSPARENT_OBJECTS_CAST_SHADOWS 0

//0 means not refractive at all
#define GOLD_REFRACTIVE EPSILON
#define JADE_REFRACTIVE EPSILON
#define MIRROR_REFRACTIVE EPSILON
#define REFRACTIVE 3.0
#define AIR_REFRACTIVE 1.0

#define MOTION_BLUR_ENABLE 0


#endif



class Vector3D {
public:
	Vector3D(); 
	Vector3D(double x, double y, double z); 
	Vector3D(const Vector3D& other);

	Vector3D& operator =(const Vector3D& other); 
	double& operator[](int i);  
	double operator[](int i) const;  

	double length() const; 
	double normalize();
	double dot(const Vector3D& other) const; 
	Vector3D cross(const Vector3D& other) const; 

	int isZero() const;

private:
	double m_data[3];
};

class Point3D {
public:
	Point3D(); 
	Point3D(double x, double y, double z);  
	Point3D(const Point3D& other); 

	Point3D& operator =(const Point3D& other); 
	double& operator[](int i); 
	double operator[](int i) const;

	Vector3D ToVector();

private:
	double m_data[3];
};

// standard operators on points and vectors
Vector3D operator *(double s, const Vector3D& v); 
Vector3D operator +(const Vector3D& u, const Vector3D& v); 
Point3D operator +(const Point3D& u, const Vector3D& v); 
Vector3D operator -(const Point3D& u, const Point3D& v); 
Vector3D operator -(const Vector3D& u, const Vector3D& v); 
Vector3D operator -(const Vector3D& u); 
Point3D operator -(const Point3D& u, const Vector3D& v); 
Vector3D cross(const Vector3D& u, const Vector3D& v); 
std::ostream& operator <<(std::ostream& o, const Point3D& p); 
std::ostream& operator <<(std::ostream& o, const Vector3D& v); 

class Vector4D {
public:
	Vector4D(); 
	Vector4D(double w, double x, double y, double z); 
	Vector4D(const Vector4D& other); 

	Vector4D& operator =(const Vector4D& other); 
	double& operator[](int i);  
	double operator[](int i) const;  

private:
	double m_data[4];
};

class Matrix4x4 {
public:
	Matrix4x4(); 
	Matrix4x4(const Matrix4x4& other); 
	Matrix4x4& operator=(const Matrix4x4& other); 

	Vector4D getRow(int row) const; 
	double *getRow(int row); 
	Vector4D getColumn(int col) const; 

	Vector4D operator[](int row) const; 
	double *operator[](int row); 

	Matrix4x4 transpose() const; 
		
private:
	double m_data[16];
};

Matrix4x4 operator *(const Matrix4x4& M, const Matrix4x4& N); 
Vector3D operator *(const Matrix4x4& M, const Vector3D& v); 
Point3D operator *(const Matrix4x4& M, const Point3D& p);
// Multiply n by the transpose of M, useful for transforming normals.  
// Recall that normals should be transformed by the inverse transpose 
// of the matrix.  
Vector3D transNorm(const Matrix4x4& M, const Vector3D& n); 
std::ostream& operator <<(std::ostream& os, const Matrix4x4& M); 

class Color {
public:
	Color(); 
	Color(double r, double g, double b); 
	Color(const Color& other); 

	Color& operator =(const Color& other); 
	Color operator *(const Color& other); 
	double& operator[](int i);  
	double operator[](int i) const; 
	
	void clamp(); 	

private:
	double m_data[3];
};

Color operator *(double s, const Color& c); 
Color operator +(const Color& u, const Color& v); 
std::ostream& operator <<(std::ostream& o, const Color& c); 

struct Material {
	Material(Color ambient, Color diffuse, Color specular, double exp, 
		double glossiness, double index) :
		ambient(ambient), diffuse(diffuse), specular(specular), 
		specular_exp(exp), glossiness(glossiness), refraction_index(index) {}
	
	// Ambient components for Phong shading.
	Color ambient; 
	// Diffuse components for Phong shading.
	Color diffuse;
	// Specular components for Phong shading.
	Color specular;
	// Specular expoent.
	double specular_exp;

	// [0,1]
	double glossiness;

	// Refraction index
	double refraction_index;

	// [0,1], 1.0 for no light passthrough, 0.0 for no light absorbtion
	double opacity = 1.0;
};

struct Intersection {
	// Location of intersection.
	Point3D point;
	// Normal at the intersection.
	Vector3D normal;
	// Material at the intersection.
	Material* mat;
	// Position of the intersection point on your ray.
	// (i.e. point = ray.origin + t_value * ray.dir)
	// This is used when you need to intersect multiply objects and
	// only want to keep the nearest intersection.
	double t_value;	
	// Set to true when no intersection has occured.
	bool none;
};

// Ray structure. 
struct Ray3D {
	Ray3D() {
		intersection.none = true; 
	}
	Ray3D( Point3D p, Vector3D v ) : origin(p), dir(v) {
		intersection.none = true;
	}
	// Origin and direction of the ray.
	Point3D origin;
	Vector3D dir;
	// Intersection status, should be computed by the intersection
	// function.
	Intersection intersection;
	// Current colour of the ray, should be computed by the shading
	// function.
	Color col;

	/*
	 * What is the index of refraction for this ray? i.e. what material is it
	 * currently inside of?
	 */
	double refraction_index = AIR_REFRACTIVE;
};

struct Camera {    
	// Camera positioned at eye, with view vector view, up vector up, and 
	// field of view fov.
	Camera(Point3D eye, Vector3D view, Vector3D up, double fov) 
	:
	eye(eye), view(view), up(up), fov(fov)
	{}
	
	// Constructs a view to world transformation matrix based on the
	// camera parameters.
	Matrix4x4 initInvViewMatrix() {
		Matrix4x4 mat; 
		Vector3D w;
		view.normalize();
		up = up - up.dot(view)*view;
		up.normalize();
		w = view.cross(up);

		mat[0][0] = w[0];
		mat[1][0] = w[1];
		mat[2][0] = w[2];
		mat[0][1] = up[0];
		mat[1][1] = up[1];
		mat[2][1] = up[2];
		mat[0][2] = -view[0];
		mat[1][2] = -view[1];
		mat[2][2] = -view[2];
		mat[0][3] = eye[0];
		mat[1][3] = eye[1];
		mat[2][3] = eye[2];

		return mat; 
	}

	Point3D eye;
	Vector3D view;
	Vector3D up;
	double fov;
};

struct Image {

	// Create an image with dimensions (width, height) 
	Image(int width, int height)
	:
	width(width), height(height), rbuffer(NULL), gbuffer(NULL), bbuffer(NULL) {
		initPixelBuffer();
	}

	~Image() {
		if (rbuffer) delete rbuffer;
		if (gbuffer) delete gbuffer;
		if (bbuffer) delete bbuffer;
	}

	// Allocates and initializes the pixel buffer for rendering, you
	// could add an interesting background to your scene by modifying 
	// this function.
	void initPixelBuffer() {
		int numbytes = width * height * sizeof(unsigned char);
		rbuffer = new unsigned char[numbytes];
		std::fill_n(rbuffer, numbytes,0);
		gbuffer = new unsigned char[numbytes];
		std::fill_n(gbuffer, numbytes,0);
		bbuffer = new unsigned char[numbytes];
		std::fill_n(bbuffer, numbytes,0);
	}

	// Saves the pixel buffer as image to a file at path
	void flushPixelBuffer(std::string path) {
		bmp_write(path.c_str(), width, height, rbuffer, gbuffer, bbuffer);
	}

	// Set color of pixel (i,j) to col
	void setColorAtPixel(int i, int j, Color& col) {
		rbuffer[i*width+j] = int(col[0]*255);
		gbuffer[i*width+j] = int(col[1]*255);
		bbuffer[i*width+j] = int(col[2]*255);
	}

	int width;
	int height;	

private:
	// Pixel buffer.
	unsigned char* rbuffer; // red channel
	unsigned char* gbuffer; // green channel
	unsigned char* bbuffer; // blue channel
};

// Returns number of intersections
// intersections must have two doubles in it
#define QUADRATIC_EPSILON 0.0001
int SolveQuadratic(double a, double b, double c, double *intersections);

// #define max(x,y) x > y ? x : y
// #define min(x,y) x < y ? x : y
Color Phong(Vector3D& _lightDirection, Vector3D& _normal, 
		Vector3D _eyeDirection, Material* mat, Color& col_ambient, 
		Color& col_diffuse, Color& col_specular);

Vector3D GetArbitraryTangentFromNorm(Vector3D& n);
