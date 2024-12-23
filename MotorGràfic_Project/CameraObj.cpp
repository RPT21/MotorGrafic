#include "CameraObj.h"

CameraObj::CameraObj(Eigen::Matrix<double, 3, 1> camera_pos, float d, int pixels_x, int pixels_y, float len_x, float len_y)
{
	// Aquesta es la funcio __init__ de la classe CameraObj
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

	this->punt_pla = camera_pos + d * vect_direct_cam;

}

void CameraObj::rotateCamera(float theta, float phi)
{
}

void CameraObj::moveCamera_rotation(float theta, float phi)
{
}

void CameraObj::rotateScreenX(float theta)
{
}

void CameraObj::rotateScreenY(float theta)
{
}

void CameraObj::followPoint(float punt[3])
{
}

void CameraObj::intrinsicRotation(float theta)
{
}

void CameraObj::moveCamera(float vect[3])
{
}
