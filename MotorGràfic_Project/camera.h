#pragma once // Aixo es per evitar que es faci un include de la mateixa llibreria multiples vegades
#include <Eigen/Dense>
#include "vec3.h"
using namespace Eigen;

// Definim les funcions que utilitzarem en camera.cpp
array<vec3, 3> R(vec3 vect, double theta);
vec3 esfericRotate(vec3 vect, double theta, double phi);
vec3 rotateAxis(vec3 point, vec3 vect, double theta);
vec3 CartesianaToEsferica(vec3 point);
vec3 EsfericaToCartesiana(vec3 point);

// Aquesta es la definicio de la classe Camera
class Camera
{
    // Definim els metodes i atributs de la classe Camera
private:
    // Atributs de la classe camera

    // Vector columna (3x1)
    // Eigen::Matrix<TipoDato, Filas, Columnas>
    // Eigen::Matrix<double, 3, 1> vector_name - Equivalent a Vector3d

    vec3 camera_pos;
    vec3 vect_direct_cam;
    vec3 coordY_pantalla;
    vec3 coordX_pantalla;
    vec3 punt_pla;
    array<double, 4> pla_camera;

    double d;
    int pixels_x;
    int pixels_y;
    double len_x;
    double len_y;
    double mida_pixel_x;
    double mida_pixel_y;

public:
    // Metodes de la classe camera

    // Constructor de la classe camera - Quan es crea un objecte de la classe camera, es crida aquest metode
    // Si no s'especifica, s'executa un constructor per defecte que es de tipus void
    Camera(vec3 camera_pos, double d, int pixels_x, int pixels_y, double len_x, double len_y);

    // Definim la resta de metodes de la classe camera
    void rotateCamera(double theta, double phi);
    void moveCamera_rotation(vec3 vect, double theta);
    void rotateScreenX(double theta);
    void rotateScreenY(double theta);
    void followPoint(vec3 punt);
    void intrinsicRotation(double theta);
    void moveCamera(vec3 vect);

	// Els metodes const nomes llegeixen els atributs de la classe, pero no els modifiquen
	// Si el const va al principi de tot, vol dir que el que retorna la funcio no es pot modificar
	// Si el const va al final, vol dir que la funcio no modifica els atributs de la classe
	// Aixo optimiza el programa, ja que el compilador sap que no es modifiquen els atributs de la classe

    vec3 getPos() const;
    vec3 getBaseX() const;
    vec3 getBaseY() const;
    vec3 getBaseZ() const;
	array<double, 4> getScreenPlane() const;
    vec3 getScreenCenter() const;
	double getLenScreenX() const;
	double getLenScreenY() const;
	double getLenPixelX() const;
	double getLenPixelY() const;
	int getScreenWidth() const;
	int getScreenHeight() const;



	// Destructor de la classe Camera - default vol dir que es el destructor per defecte, que no fa res, i es crida quan es destrueix l'objecte
    ~Camera() = default;
};
