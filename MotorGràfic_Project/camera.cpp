#include "camera.h"
#include <math.h>
#include "vec3.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;
using namespace Eigen;

array<vec3, 3> R(vec3 vect, double theta) {
	// Matrix2d es equivalent a una matriu de 2x2 de tipus float.
	// Estem especificant que la funcio R retorna una matriu de 2x2 de tipus float.

	// Fem unitari el vector
	vect = unit_vector(vect);
	double Vx, Vy, Vz;

	Vx = vect[0];
	Vy = vect[1];
	Vz = vect[2];

	// Calculem els seno i coseno de l'angle
	double c = cos(theta);
	double s = sin(theta);

	array<vec3, 3> result;
	result[0] = { c + (1 - c) * Vx * Vx, (1 - c) * Vx * Vy - s * Vz, (1 - c) * Vx * Vz + s * Vy };
	result[1] = { (1 - c) * Vx * Vy + s * Vz, c + (1 - c) * Vy * Vy, (1 - c) * Vy * Vz - s * Vx };
	result[2] = { (1 - c) * Vx * Vz - s * Vy, (1 - c) * Vy * Vz + s * Vx, c + (1 - c) * Vz * Vz };

	return result;

}

vec3 esfericRotate(vec3 vect, double theta, double phi) {

	// Calculem els seno i coseno de l'angle
	double c = cos(phi);
	double s = sin(phi);

	vec3 vect_director_rotacio_eix_theta = { -s, c, 0 };
	array<vec3, 3> R_phi = R({ 0, 0, 1 }, phi);
	array<vec3, 3> R_theta = R(vect_director_rotacio_eix_theta, theta);
	vect = matrix_dot_vector(R_phi, vect);
	vect = matrix_dot_vector(R_theta, vect);
	return vect;

}

vec3 rotateAxis(vec3 point, vec3 vect, double theta) {
	array<vec3, 3> R_matrix = R(vect, theta);
	return matrix_dot_vector(R_matrix, point);
}

int sign(double x) {
	if (x > 0) return 1;  // Valor positiu
	if (x < 0) return -1; // Valor negatiu
	return 0;              // Zero
}

vec3 CartesianaToEsferica(vec3 point) {
	double theta, phi;

	double x = point[0];
	double y = point[1];
	double z = point[2];
	double r = sqrt(x * x + y * y + z * z);

	if (z > 0.0) {
		theta = atan(sqrt(x * x + y * y) / z);
	}
	else if (z == 0) {
		theta = M_PI / 2;
	}
	else {
		theta = atan(sqrt(x * x + y * y) / z) + M_PI;
	}

	if (x > 0 and y > 0) {
		phi = atan(y / x);
	}
	else if (x > 0 and y < 0) {
		phi = atan(y / x) + 2 * M_PI;
	}
	else if (x == 0) {
		phi = M_PI / 2 * sign(y);
	}
	else if (x < 0) {
		phi = atan(y / x) + M_PI;
	}

	vec3 result = { r, theta, phi };
	return result;
}

vec3 EsfericaToCartesiana(vec3 point) {
	double r = point[0];
	double theta = point[1];
	double phi = point[2];
	double x = r * sin(theta) * cos(phi);
	double y = r * sin(theta) * sin(phi);
	double z = r * cos(theta);
	vec3 result = { x, y, z };
	return result;
}


Camera::Camera(vec3 camera_pos, double d, int pixels_x, int pixels_y, double len_x, double len_y)
{
	// Aquesta es la funcio __init__ de la classe camera
	// Com que camera_pos te el mateix nom que l'atribut de la classe, s'ha d'usar this-> per referir-se a l'atribut de la classe
	this->camera_pos = camera_pos;
    this->d = d;
    this->pixels_x = pixels_x;
    this->pixels_y = pixels_y;
    this->len_x = len_x;
    this->len_y = len_y;

    mida_pixel_x = len_x / pixels_x;
	mida_pixel_y = len_y / pixels_y;

	vect_direct_cam = { 1, 0, 0 };
	coordY_pantalla = { 0, 0, 1 };
	coordX_pantalla = { 0, -1, 0 };

	punt_pla = camera_pos + d * vect_direct_cam;

	double D = -vect_direct_cam[0] * punt_pla[0] - vect_direct_cam[1] * punt_pla[1] - vect_direct_cam[2] * punt_pla[2];
	pla_camera = { vect_direct_cam[0], vect_direct_cam[1], vect_direct_cam[2] , D};

}

void Camera::rotateCamera(double theta, double phi)
{
	vect_direct_cam = esfericRotate(vect_direct_cam, theta, phi);
	coordY_pantalla = esfericRotate(coordY_pantalla, theta, phi);
	coordX_pantalla = esfericRotate(coordX_pantalla, theta, phi);

	punt_pla = camera_pos + d * vect_direct_cam;
	double D = -vect_direct_cam[0] * punt_pla[0] - vect_direct_cam[1] * punt_pla[1] - vect_direct_cam[2] * punt_pla[2];
	pla_camera = { vect_direct_cam[0], vect_direct_cam[1], vect_direct_cam[2] , D };
}

void Camera::moveCamera_rotation(vec3 vect, double theta)
{
	camera_pos = rotateAxis(vect_direct_cam, vect, theta);

	vect_direct_cam = rotateAxis(vect_direct_cam, vect, theta);
	coordX_pantalla = rotateAxis(coordX_pantalla, vect, theta);
	coordY_pantalla = rotateAxis(coordY_pantalla, vect, theta);

	punt_pla = camera_pos + d * vect_direct_cam;

	double D = -vect_direct_cam[0] * punt_pla[0] - vect_direct_cam[1] * punt_pla[1] - vect_direct_cam[2] * punt_pla[2];
	pla_camera = { vect_direct_cam[0], vect_direct_cam[1], vect_direct_cam[2] , D };
}

void Camera::rotateScreenX(double theta)
{
	vect_direct_cam = rotateAxis(vect_direct_cam, coordX_pantalla, theta);
	coordX_pantalla = rotateAxis(coordX_pantalla, coordX_pantalla, theta);
	coordY_pantalla = rotateAxis(coordY_pantalla, coordX_pantalla, theta);

	punt_pla = camera_pos + d * vect_direct_cam;

	double D = -vect_direct_cam[0] * punt_pla[0] - vect_direct_cam[1] * punt_pla[1] - vect_direct_cam[2] * punt_pla[2];
	pla_camera = { vect_direct_cam[0], vect_direct_cam[1], vect_direct_cam[2] , D };
}

void Camera::rotateScreenY(double theta)
{
	vect_direct_cam = rotateAxis(vect_direct_cam, coordY_pantalla, theta);
	coordX_pantalla = rotateAxis(coordX_pantalla, coordY_pantalla, theta);
	coordY_pantalla = rotateAxis(coordY_pantalla, coordY_pantalla, theta);

	punt_pla = camera_pos + d * vect_direct_cam;

	double D = -vect_direct_cam[0] * punt_pla[0] - vect_direct_cam[1] * punt_pla[1] - vect_direct_cam[2] * punt_pla[2];
	pla_camera = { vect_direct_cam[0], vect_direct_cam[1], vect_direct_cam[2] , D };
}

void Camera::followPoint(vec3 punt)
{
	punt = punt - camera_pos;
	vec3 punt_esferiques = CartesianaToEsferica(punt);
	double theta = punt_esferiques[1];
	double phi = punt_esferiques[2];

	vect_direct_cam = esfericRotate({ 1, 0, 0 }, theta - M_PI / 2, phi);
	coordY_pantalla = esfericRotate({ 0, 0, 1 }, theta - M_PI / 2, phi);
	coordX_pantalla = esfericRotate({ 0, -1, 0 }, theta - M_PI / 2, phi);

	punt_pla = camera_pos + d * vect_direct_cam;

	double D = -vect_direct_cam[0] * punt_pla[0] - vect_direct_cam[1] * punt_pla[1] - vect_direct_cam[2] * punt_pla[2];
	pla_camera = { vect_direct_cam[0], vect_direct_cam[1], vect_direct_cam[2] , D };
}

void Camera::intrinsicRotation(double theta)
{
	coordX_pantalla = rotateAxis(coordX_pantalla, vect_direct_cam, theta);
	coordY_pantalla = rotateAxis(coordY_pantalla, vect_direct_cam, theta);
}

void Camera::moveCamera(vec3 vect)
{
	double inc_y = vect[0];
	double inc_x = -vect[1];
	double inc_z = vect[2];

	vec3 vector_translacio = inc_x*coordX_pantalla + inc_y*coordY_pantalla + inc_z*vect_direct_cam;
	camera_pos = camera_pos + vector_translacio;

	punt_pla = camera_pos + d * vect_direct_cam;

	double D = -vect_direct_cam[0] * punt_pla[0] - vect_direct_cam[1] * punt_pla[1] - vect_direct_cam[2] * punt_pla[2];
	pla_camera = { vect_direct_cam[0], vect_direct_cam[1], vect_direct_cam[2] , D };

}

vec3 Camera::getPos() const 
{
	return camera_pos;
}

vec3 Camera::getBaseX() const
{
	return coordX_pantalla;
}

vec3 Camera::getBaseY() const
{
	return coordY_pantalla;
}

vec3 Camera::getBaseZ() const
{
	return vect_direct_cam;
}

array<double, 4> Camera::getScreenPlane() const
{
	return pla_camera;
}

vec3 Camera::getScreenCenter() const
{
	return punt_pla;
}

double Camera::getLenScreenX() const
{
	return len_x;
}

double Camera::getLenScreenY() const
{
	return len_y;
}

double Camera::getLenPixelX() const
{
	return mida_pixel_x;
}

double Camera::getLenPixelY() const
{
	return mida_pixel_y;
}

int Camera::getScreenWidth() const
{
	return pixels_x;
}

int Camera::getScreenHeight() const
{
	return pixels_y;
}

