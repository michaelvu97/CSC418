/***********************************************************

	Starter code for Assignment 3
	
	Implementations of util.h	

***********************************************************/

#include "util.h"

Point3D::Point3D() {
	m_data[0] = 0.0;
	m_data[1] = 0.0;
	m_data[2] = 0.0;
}

Point3D::Point3D(double x, double y, double z) { 
	m_data[0] = x;
	m_data[1] = y;
	m_data[2] = z;
}

Point3D::Point3D(const Point3D& other) {
	m_data[0] = other.m_data[0];
	m_data[1] = other.m_data[1];
	m_data[2] = other.m_data[2];
}

Point3D& Point3D::operator =(const Point3D& other) {
	m_data[0] = other.m_data[0];
	m_data[1] = other.m_data[1];
	m_data[2] = other.m_data[2];
	return *this;
}

double& Point3D::operator[](int i) {
	return m_data[i];
}

double Point3D::operator[](int i) const {
	return m_data[i];
}
	
Vector3D Point3D::ToVector() {
	return Vector3D(m_data[0], m_data[1], m_data[2]);
}

Vector3D::Vector3D() {
	m_data[0] = 0.0;
	m_data[1] = 0.0;
	m_data[2] = 0.0;
}

Vector3D::Vector3D(double x, double y, double z) { 
	m_data[0] = x;
	m_data[1] = y;
	m_data[2] = z;
}

Vector3D::Vector3D(const Vector3D& other) {
	m_data[0] = other.m_data[0];
	m_data[1] = other.m_data[1];
	m_data[2] = other.m_data[2];
}

Vector3D& Vector3D::operator =(const Vector3D& other) {
	m_data[0] = other.m_data[0];
	m_data[1] = other.m_data[1];
	m_data[2] = other.m_data[2];
	return *this;
}

double& Vector3D::operator[](int i) {
	return m_data[i];
}
double Vector3D::operator[](int i) const {
	return m_data[i];
}

double Vector3D::length() const
{
	return sqrt(dot(*this));
}

double Vector3D::normalize() {
	double denom = 1.0;
	double x = (m_data[0] > 0.0) ? m_data[0] : -m_data[0];
	double y = (m_data[1] > 0.0) ? m_data[1] : -m_data[1];
	double z = (m_data[2] > 0.0) ? m_data[2] : -m_data[2];

	if(x > y) {
		if(x > z) {
			if(1.0 + x > 1.0) {
				y = y / x;
				z = z / x;
				denom = 1.0 / (x * sqrt(1.0 + y*y + z*z));
			}
		} else { /* z > x > y */ 
			if(1.0 + z > 1.0) {
				y = y / z;
				x = x / z;
				denom = 1.0 / (z * sqrt(1.0 + y*y + x*x));
			}
		}
	} else {
		if(y > z) {
			if(1.0 + y > 1.0) {
				z = z / y;
				x = x / y;
				denom = 1.0 / (y * sqrt(1.0 + z*z + x*x));
			}
		} else { /* x < y < z */
			if(1.0 + z > 1.0) {
				y = y / z;
				x = x / z;
				denom = 1.0 / (z * sqrt(1.0 + y*y + x*x));
			}
		}
	}

	if(1.0 + x + y + z > 1.0) {
		m_data[0] *= denom;
		m_data[1] *= denom;
		m_data[2] *= denom;
		return 1.0 / denom;
	}

	return 0.0;
}

double Vector3D::dot(const Vector3D& other) const
{
	return m_data[0]*other.m_data[0] + 
		m_data[1]*other.m_data[1] + 
		m_data[2]*other.m_data[2];
}

Vector3D Vector3D::cross(const Vector3D& other) const
{
	return Vector3D(
			m_data[1]*other[2] - m_data[2]*other[1],
			m_data[2]*other[0] - m_data[0]*other[2],
			m_data[0]*other[1] - m_data[1]*other[0]);
}

Vector3D operator *(double s, const Vector3D& v)
{
  return Vector3D(s*v[0], s*v[1], s*v[2]);
}

Vector3D operator +(const Vector3D& u, const Vector3D& v)
{
  return Vector3D(u[0]+v[0], u[1]+v[1], u[2]+v[2]);
}

Point3D operator +(const Point3D& u, const Vector3D& v)
{
  return Point3D(u[0]+v[0], u[1]+v[1], u[2]+v[2]);
}

Vector3D operator -(const Point3D& u, const Point3D& v)
{
  return Vector3D(u[0]-v[0], u[1]-v[1], u[2]-v[2]);
}

Vector3D operator -(const Vector3D& u, const Vector3D& v)
{
  return Vector3D(u[0]-v[0], u[1]-v[1], u[2]-v[2]);
}

Vector3D operator -(const Vector3D& u)
{
  return Vector3D(-u[0], -u[1], -u[2]);
}

Point3D operator -(const Point3D& u, const Vector3D& v)
{
  return Point3D(u[0]-v[0], u[1]-v[1], u[2]-v[2]);
}

Vector3D cross(const Vector3D& u, const Vector3D& v) 
{
  return u.cross(v);
}

std::ostream& operator <<(std::ostream& s, const Point3D& p)
{
  return s << "p(" << p[0] << "," << p[1] << "," << p[2] << ")";
}

std::ostream& operator <<(std::ostream& s, const Vector3D& v)
{
  return s << "v(" << v[0] << "," << v[1] << "," << v[2] << ")";
}

Color::Color() {
	m_data[0] = 0.0;
	m_data[1] = 0.0;
	m_data[2] = 0.0;
}

Color::Color(double r, double g, double b) { 
	m_data[0] = r;
	m_data[1] = g;
	m_data[2] = b;
}

Color::Color(const Color& other) {
	m_data[0] = other.m_data[0];
	m_data[1] = other.m_data[1];
	m_data[2] = other.m_data[2];
}

Color& Color::operator =(const Color& other) {
	m_data[0] = other.m_data[0];
	m_data[1] = other.m_data[1];
	m_data[2] = other.m_data[2];
	return *this;
}

Color Color::operator *(const Color& other) {
	return Color(m_data[0]*other.m_data[0], 
		m_data[1]*other.m_data[1], 
		m_data[2]*other.m_data[2]);
}

double& Color::operator[](int i) {
	return m_data[i];
}
double Color::operator[](int i) const {
	return m_data[i];
}

void Color::clamp() {
	for (int i = 0; i < 3; i++) {
		if (std::isnan(m_data[i])) m_data[i] = 0.0;
		if (m_data[i] > 1.0) m_data[i] = 1.0; 
		if (m_data[i] < 0.0) m_data[i] = 0.0; 

	}
}

Color operator *(double s, const Color& c)
{
  return Color(s*c[0], s*c[1], s*c[2]);
}

Color operator +(const Color& u, const Color& v)
{
  return Color(u[0]+v[0], u[1]+v[1], u[2]+v[2]);
}

std::ostream& operator <<(std::ostream& s, const Color& c)
{
  return s << "c(" << c[0] << "," << c[1] << "," << c[2] << ")";
}

Vector4D::Vector4D() {
	m_data[0] = 0.0;
	m_data[1] = 0.0;
	m_data[2] = 0.0;
	m_data[3] = 0.0;
}

Vector4D::Vector4D(double w, double x, double y, double z) { 
	m_data[0] = w;
	m_data[1] = x;
	m_data[2] = y;
	m_data[3] = z;
}

Vector4D::Vector4D(const Vector4D& other) {
	m_data[0] = other.m_data[0];
	m_data[1] = other.m_data[1];
	m_data[2] = other.m_data[2];
	m_data[3] = other.m_data[3];
}

Vector4D& Vector4D::operator =(const Vector4D& other) {
	m_data[0] = other.m_data[0];
	m_data[1] = other.m_data[1];
	m_data[2] = other.m_data[2];
	m_data[3] = other.m_data[3];
	return *this;
}

double& Vector4D::operator[](int i) {
	return m_data[i];
}
double Vector4D::operator[](int i) const {
	return m_data[i];
}

Matrix4x4::Matrix4x4() {
	for (int i = 0; i < 16; i++) 
		m_data[i] = 0.0; 
	m_data[0] = 1.0;
	m_data[5] = 1.0;
	m_data[10] = 1.0;
	m_data[15] = 1.0;
}

Matrix4x4::Matrix4x4(const Matrix4x4& other) {
	for (int i = 0; i < 16; i++) 
		m_data[i] = other.m_data[i]; 
}

Matrix4x4& Matrix4x4::operator=(const Matrix4x4& other) {
	for (int i = 0; i < 16; i++) 
		m_data[i] = other.m_data[i]; 
	return *this;
}

Vector4D Matrix4x4::getRow(int row) const {
	return Vector4D(m_data[4*row], m_data[4*row+1], m_data[4*row+2], 
			m_data[4*row+3]);
}

double* Matrix4x4::getRow(int row) {
	return (double*)m_data + 4*row;
}

Vector4D Matrix4x4::getColumn(int col) const {
	return Vector4D(m_data[col], m_data[4+col], m_data[8+col], 
			m_data[12+col]);
}

Vector4D Matrix4x4::operator[](int row) const {
	return getRow(row);
}

double* Matrix4x4::operator[](int row) {
	return getRow(row);
}

Matrix4x4 Matrix4x4::transpose() const {
	Matrix4x4 M; 
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			M[i][j] = (*this)[j][i]; 
		}
	}
	return M; 
}
 
Matrix4x4 operator *(const Matrix4x4& a, const Matrix4x4& b) {
	Matrix4x4 ret;

	for(size_t i = 0; i < 4; ++i) {
		Vector4D row = a.getRow(i);

		for(size_t j = 0; j < 4; ++j) {
			ret[i][j] = row[0] * b[0][j] + row[1] * b[1][j] + 
				row[2] * b[2][j] + row[3] * b[3][j];
		}
	}

	return ret;
}

Vector3D operator *(const Matrix4x4& M, const Vector3D& v) {
	return Vector3D(
			v[0] * M[0][0] + v[1] * M[0][1] + v[2] * M[0][2],
			v[0] * M[1][0] + v[1] * M[1][1] + v[2] * M[1][2],
			v[0] * M[2][0] + v[1] * M[2][1] + v[2] * M[2][2]);
}

Point3D operator *(const Matrix4x4& M, const Point3D& p) {
	return Point3D(
			p[0] * M[0][0] + p[1] * M[0][1] + p[2] * M[0][2] + M[0][3],
			p[0] * M[1][0] + p[1] * M[1][1] + p[2] * M[1][2] + M[1][3],
			p[0] * M[2][0] + p[1] * M[2][1] + p[2] * M[2][2] + M[2][3]);
}

Vector3D transNorm(const Matrix4x4& M, const Vector3D& n) {
	return Vector3D(
			n[0] * M[0][0] + n[1] * M[1][0] + n[2] * M[2][0],
			n[0] * M[0][1] + n[1] * M[1][1] + n[2] * M[2][1],
			n[0] * M[0][2] + n[1] * M[1][2] + n[2] * M[2][2]);
}

std::ostream& operator <<(std::ostream& os, const Matrix4x4& M) {
	return os << "[" << M[0][0] << " " << M[0][1] << " " 
		<< M[0][2] << " " << M[0][3] << "]" << std::endl
		<< "[" << M[1][0] << " " << M[1][1] << " " 
		<< M[1][2] << " " << M[1][3] << "]" << std::endl
		<< "[" << M[2][0] << " " << M[2][1] << " " 
		<< M[2][2] << " " << M[2][3] << "]" << std::endl
		<< "[" << M[3][0] << " " << M[3][1] << " " 
		<< M[3][2] << " " << M[3][3] << "]";
}

int SolveQuadratic(double a, double b, double c, double *intersections) {

	double thing = (b*b) - (4.0 * a * c);

	if (thing < 0)
		return 0;

	if (thing > -QUADRATIC_EPSILON && thing < QUADRATIC_EPSILON) {
		intersections[0] = (-b) / (2.0 * a);
		return 1;
	}

	intersections[0] = (-b + sqrt(thing))/ (2.0 * a);
	intersections[1] = (-b - sqrt(thing))/ (2.0 * a);
	return 2;
}
	
/*
 * Light direction is from the intersection point to the light
 * Eye direction is from the eye to the point.
 */
Color Phong(Vector3D& _lightDirection, Vector3D& _normal, Vector3D _eyeDirection,
	Material* mat, Color& col_ambient, Color& col_diffuse, Color& col_specular) {

	_lightDirection.normalize();
	double diffuseIntensity = 
			std::max(0.0, _normal.dot(_lightDirection));

	Vector3D mirrorRay = (
			(2.0 * _normal.dot(_lightDirection) * _normal)
			 - _lightDirection
	);
	mirrorRay.normalize();

	// Just in case
	_eyeDirection.normalize();

	double specularIntensity = pow(
			std::max(0.0, _eyeDirection.dot(-1 * mirrorRay)), 
			mat -> specular_exp
	);

	Color diffuseColor = diffuseIntensity * col_diffuse;
	diffuseColor.clamp();
	Color specularColor = specularIntensity * col_specular;
	specularColor.clamp();

	Color c(
			(col_ambient * mat -> ambient) + 
			(diffuseColor * mat -> diffuse) + 
			(specularColor * mat -> specular)
	);
	c.clamp();
	return c;
}

int Vector3D::isZero() const {
	return (this -> m_data[0] < EPSILON &&
			this -> m_data[1] < EPSILON &&
			this -> m_data[2] < EPSILON);
}

Vector3D GetArbitraryTangentFromNorm(Vector3D& n) {
	Vector3D test(1.0, 1.0, 1.0);


	Vector3D crossRes = n.cross(test);

	if (crossRes.isZero()) {
		test[0] = 0.0;

		crossRes = n.cross(test);
	}

	crossRes.normalize();

	return crossRes;

}

unsigned char* readBMP(char* filename) {
	/*
	 * Note that this is taken from StackOverflow @9296059 as a helper class for
	 * loading bitmaps.
	 * 
	 */

	FILE* f = fopen(filename, "rb");
	unsigned char info[122];
	fread(info, sizeof(unsigned char), 122, f); // Read 122-byte header.
	// This might be sketchy since the header size could change.

	// TODO make this static.
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

	int size = 3 * width * height; // 24 bit color depth

	unsigned char* data = new unsigned char[size];
	fread(data, sizeof(unsigned char), size, f);
	fclose(f);

	/*
	 * Convert from BGR to RGB? 
	 */
	for (int i = 0; i < size; i += 3) {
		unsigned char tmp = data[i];
		data[i] = data[i+2];
		data[i + 2] = tmp;
	}

	return data;

}

