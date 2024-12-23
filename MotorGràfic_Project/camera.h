#pragma once // Aixo es per evitar que es faci un include de la mateixa llibreria multiples vegades
#include <Eigen/Dense>
using namespace Eigen;

// Definim les funcions que utilitzarem en camera.cpp
Matrix3d R(Matrix<double, 3, 1> vect, double theta);
Matrix<double, 3, 1> esfericRotate(Matrix<double, 3, 1> vect, double theta, double phi);
Matrix<double, 3, 1> rotateAxis(Matrix<double, 3, 1> point, Matrix<double, 3, 1> vect, double theta);
Matrix<double, 3, 1> CartesianaToEsferica(Matrix<double, 3, 1> point);
Matrix<double, 3, 1> EsfericaToCartesiana(Matrix<double, 3, 1> point);

// Aquesta es la definicio de la classe camera
class camera
{
    // Definim els metodes i atributs de la classe camera
private:
    // Atributs de la classe camera

    // Vector columna (3x1)
    // Eigen::Matrix<TipoDato, Filas, Columnas>
    // Eigen::Matrix<double, 3, 1> vector_name

    Matrix<double, 3, 1> camera_pos;
    Matrix<double, 3, 1> vect_direct_cam;
    Matrix<double, 3, 1> coordY_pantalla;
    Matrix<double, 3, 1> coordX_pantalla;
    Matrix<double, 3, 1> punt_pla;
    Matrix<double, 4, 1> pla_camera;

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
    camera(Eigen::Matrix<double, 3, 1> camera_pos, double d, int pixels_x, int pixels_y, double len_x, double len_y);

    // Definim la resta de metodes de la classe camera
    void rotateCamera(double theta, double phi);
    void moveCamera_rotation(Matrix<double, 3, 1> vect, double theta);
    void rotateScreenX(double theta);
    void rotateScreenY(double theta);
    void followPoint(Matrix<double, 3, 1> punt);
    void intrinsicRotation(double theta);
    void moveCamera(Matrix<double, 3, 1> vect);

	// Destructor de la classe camera - default vol dir que es el destructor per defecte, que no fa res, i es crida quan es destrueix l'objecte
    ~camera() = default;
};
