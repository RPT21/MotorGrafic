#pragma once // Aixo es per evitar que es faci un include de la mateixa llibreria multiples vegades
# include <array>
# include <Eigen/Dense>
class CameraObj
{
	// Definim els metodes i atributs de la classe CameraObj
    private:
		// Atributs de la classe CameraObj

        // Vector columna (3x1)
        // Eigen::Matrix<TipoDato, Filas, Columnas>
        // Eigen::Matrix<double, 3, 1> vector_name

        Eigen::Matrix<double, 3, 1> camera_pos;
        Eigen::Matrix<double, 3, 1> vect_direct_cam;
        Eigen::Matrix<double, 3, 1> coordY_pantalla;
        Eigen::Matrix<double, 3, 1> coordX_pantalla;
        Eigen::Matrix<double, 3, 1> punt_pla;
        Eigen::Matrix<double, 3, 1> pla_camera;

        float d;
        int pixels_x;
        int pixels_y;
        float len_x;
        float len_y;
        float mida_pixel_x;
        float mida_pixel_y;

    public:
		// Metodes de la classe CameraObj
     
		// Constructor de la classe CameraObj - Quan es crea un objecte de la classe CameraObj, es crida aquest metode
        CameraObj(Eigen::Matrix<double, 3, 1> camera_pos, float d, int pixels_x, int pixels_y, float len_x, float len_y);

        void rotateCamera(float theta, float phi);
		void moveCamera_rotation(float theta, float phi);
        void rotateScreenX(float theta);
		void rotateScreenY(float theta);
        void followPoint(float punt[3]);
        void intrinsicRotation(float theta);
        void moveCamera(float vect[3]);
};
// Aquesta es la definicio de la classe CameraObj