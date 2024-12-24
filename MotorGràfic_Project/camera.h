#pragma once // Aixo es per evitar que es faci un include de la mateixa llibreria multiples vegades
#include <Eigen/Dense>
using namespace Eigen;

// Definim les funcions que utilitzarem en camera.cpp
Matrix3d R(Vector3d vect, double theta);
Vector3d esfericRotate(Vector3d vect, double theta, double phi);
Vector3d rotateAxis(Vector3d point, Vector3d vect, double theta);
Vector3d CartesianaToEsferica(Vector3d point);
Vector3d EsfericaToCartesiana(Vector3d point);

// Aquesta es la definicio de la classe Camera
class Camera
{
    // Definim els metodes i atributs de la classe Camera
private:
    // Atributs de la classe camera

    // Vector columna (3x1)
    // Eigen::Matrix<TipoDato, Filas, Columnas>
    // Eigen::Matrix<double, 3, 1> vector_name - Equivalent a Vector3d

    Vector3d camera_pos;
    Vector3d vect_direct_cam;
    Vector3d coordY_pantalla;
    Vector3d coordX_pantalla;
    Vector3d punt_pla;
    Vector4d pla_camera;

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
    Camera(Vector3d camera_pos, double d, int pixels_x, int pixels_y, double len_x, double len_y);

    // Definim la resta de metodes de la classe camera
    void rotateCamera(double theta, double phi);
    void moveCamera_rotation(Vector3d vect, double theta);
    void rotateScreenX(double theta);
    void rotateScreenY(double theta);
    void followPoint(Vector3d punt);
    void intrinsicRotation(double theta);
    void moveCamera(Vector3d vect);

	// Els metodes const nomes llegeixen els atributs de la classe, pero no els modifiquen
	// Si el const va al principi de tot, vol dir que el que retorna la funcio no es pot modificar
	// Si el const va al final, vol dir que la funcio no modifica els atributs de la classe
	// Aixo optimiza el programa, ja que el compilador sap que no es modifiquen els atributs de la classe

    Vector3d getPos() const;
    Vector3d getBaseX() const;
    Vector3d getBaseY() const;
    Vector3d getBaseZ() const;
	Vector4d getScreenPlane() const;
	Vector3d getScreenCenter() const;
	double getLenScreenX() const;
	double getLenScreenY() const;
	double getLenPixelX() const;
	double getLenPixelY() const;
	int getScreenWidth() const;
	int getScreenHeight() const;



	// Destructor de la classe Camera - default vol dir que es el destructor per defecte, que no fa res, i es crida quan es destrueix l'objecte
    ~Camera() = default;
};
