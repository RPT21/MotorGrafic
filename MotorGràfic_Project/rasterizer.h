#pragma once
#include <vector>
#include <Eigen/Dense>
#include "camera.h"
#include "scene.h"
using namespace Eigen;
using namespace std;

void rasterizeImage(vector<unsigned char>& image, Camera& camera, Scene& scene) {

	// size_t es un tipus de variable entera sense signe de gran capacitat, retornada per size() o sizeof()
	size_t number_triangles = scene.triangles.size();

	// Constant() es un mètode estatic de la classe Vector3d que retorna un vector columna amb els valors de l'argument
	// Per accedir a metodes estatics s'utilitza el nom de la classe seguit de dos punts i el nom del metode
	// Per accedir a metodes no estatics s'utilitza el nom de l'objecte seguit de un punt i el nom del metode
	// La diferencia entre metodes estatics i no estatics es que els estatics no necessiten un objecte per ser cridats
	// Sino que es criden directament des del nom de la classe, no des del nom de l'objecte que has creat a partir de la classe.

	Vector3d camera_pos = camera.getPos();
		Vector3d Cx = Vector3d::Constant(camera_pos[0]);
		Vector3d Cy = Vector3d::Constant(camera_pos[1]);
		Vector3d Cz = Vector3d::Constant(camera_pos[2]);

	Vector3d screen_center = camera.getScreenCenter();
		Vector3d Sx = Vector3d::Constant(screen_center[0]);
		Vector3d Sy = Vector3d::Constant(screen_center[1]);
		Vector3d Sz = Vector3d::Constant(screen_center[2]);

	Vector4d pla_camera = camera.getScreenPlane();
		double A = pla_camera[0];
		double B = pla_camera[1];
		double C = pla_camera[2];
		Vector3d D = Vector3d::Constant(pla_camera[3]);

	Vector3d lenScreen_x = Vector3d::Constant(camera.getLenScreenX());
	Vector3d lenScreen_y = Vector3d::Constant(camera.getLenScreenY());

	Vector3d lenPixel_x = Vector3d::Constant(camera.getLenPixelX());
	Vector3d lenPixel_y = Vector3d::Constant(camera.getLenPixelY());

	Vector3d Base_x = camera.getBaseX();
	Vector3d Base_y = camera.getBaseY();

	for (int i = 0; i < number_triangles; i++) {

		Matrix3d v_direct = scene.triangles[i].vertexs.colwise() - camera_pos;
		// Normalitzem columna a columna i extreiem les components
			v_direct = v_direct.colwise().normalized();
				Vector3d Vx = v_direct.row(0);
				Vector3d Vy = v_direct.row(1);
				Vector3d Vz = v_direct.row(2);

		Vector3d numerador = -(A * Cx + B * Cy + C * Cz + D);
		Vector3d denominador = A * Vx + B * Vy + C * Vz;

		// cwiseInverse() es un metode de la classe Vector3d que retorna un vector columna amb els valors de l'inversa de cada element del vector
        Vector3d t = numerador.cwiseProduct(denominador.cwiseInverse());

		// Per altra banda, cwizeProduct() es un metode de la classe Vector3d que retorna un vector columna amb els valors de la multiplicacio element a element dels dos vectors
		// Quan es fa un producte de dos vectors, per defecte es fa el producte escalar, pero si es vol fer el producte element a element s'ha d'utilitzar el metode cwiseProduct()

		Vector3d inter_x = Cx + t.cwiseProduct(Vx);
		Vector3d inter_y = Cy + t.cwiseProduct(Vy);
		Vector3d inter_z = Cz + t.cwiseProduct(Vz);

		Vector3d pant_x = (inter_x - Sx) * Base_x[0] + (inter_y - Sy) * Base_x[1] + (inter_z - Sz) * Base_x[2];
		Vector3d pant_y = (inter_x - Sx) * Base_y[0] + (inter_y - Sy) * Base_y[1] + (inter_z - Sz) * Base_y[2];

		pant_x = pant_x + lenScreen_x / 2;
		pant_y = pant_y + lenScreen_y / 2;

		// Per poder fer el round() s'ha de convertir el vector en un array, ja que el metode round() no esta definit per vectors
		// Per convertir un vector en un array s'utilitza el metode array(), i per convertir un array en un vector s'utilitza el metode matrix()
		// Aixo es fa perque els vectors no tenen totes les funcions que tenen els arrays, i per tant, s'han de convertir en arrays per poder utilitzar aquestes funcions

		Vector3d pixel_coords_x = (pant_x.cwiseProduct(lenPixel_x.cwiseInverse())).array().round().matrix();
		Vector3d pixel_coords_y = (pant_y.cwiseProduct(lenPixel_y.cwiseInverse())).array().round().matrix();





		

	}
}

