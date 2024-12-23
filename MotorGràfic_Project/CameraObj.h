#pragma once // Aixo es per evitar que es faci un include de la mateixa llibreria multiples vegades
# include <array>
# include <Eigen/Dense>
class CameraObj
{
	// Definim els metodes i atributs de la classe CameraObj
    private:
		// Atributs de la classe CameraObj
        std::array<float, 3> camera_pos;
        std::array<float, 3> vect_direct_cam;
        std::array<float, 3> coordY_pantalla;
        std::array<float, 3> coordX_pantalla;
        std::array<float, 3> punt_pla;
        std::array<float, 4> pla_camera;

		// std::array<std::array<float, 3>, 3> matriu_3_3; - Per definir una matriu 3x3

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
		CameraObj(std::array<float, 3> camera_pos, float d, int pixels_x, int pixels_y, float len_x, float len_y);
		
        void rotateCamera(float theta, float phi);
		void moveCamera_rotation(float theta, float phi);
        void rotateScreenX(float theta);
		void rotateScreenY(float theta);
        void followPoint(float punt[3]);
        void intrinsicRotation(float theta);
        void moveCamera(float vect[3]);
};
// Aquesta es la definicio de la classe CameraObj