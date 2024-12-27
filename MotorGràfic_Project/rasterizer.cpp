#include <vector>
#include <Eigen/Dense>
#include "camera.h"
#include "scene.h"
#include "rasterizer.h"
#include <algorithm>
#include <execution>
#include "vec3.h"

using namespace Eigen;
using namespace std;

void TraceLine(vector<unsigned char>& image, Camera& camera, int x0, int y0, int x1, int y1)
{
	// Definim les variables que utilitzarem per a l'algoritme de Bresenham
	int width = camera.getScreenWidth();
	int height = camera.getScreenHeight();

	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;
	int err = dx - dy;
	int e2;

	if (x0 < 0 || x0 >= width || y0 < 0 || y0 >= height || x1 < 0 || x1 >= width || y1 < 0 || y1 >= height) {
		return;
	}
	else if (x0 < 0 || x0 >= width || y0 < 0 || y0 >= height) {
		swap(x0, x1);
		swap(y0, y1);
	}

	// Bucle per a recorrer tots els pixels de la linia
	while (true) {
		// Pintem el pixel actual
		if (x0 >= 0 && x0 < width && y0 >= 0 && y0 < height) {
			image[3 * (x0 + y0 * width)] = 0;
			image[3 * (x0 + y0 * width) + 1] = 0;
			image[3 * (x0 + y0 * width) + 2] = 0;
		}
		else {
			break;
		}

		// Si hem arribat al final de la linia, sortim del bucle
		if (x0 == x1 && y0 == y1) break;

		// Calculem la seguent posicio de la linia
		e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y0 += sy;
		}
	}
}

void rasterizeImage(vector<unsigned char>& image, Camera& camera, Scene& scene) {
	
	fill(image.begin(), image.end(), 255);

	// size_t es un tipus de variable entera sense signe de gran capacitat, retornada per size() o sizeof()
	size_t number_triangles = scene.triangles.size();

	// Constant() es un mètode estatic de la classe Vector3d que retorna un vector columna amb els valors de l'argument
	// Per accedir a metodes estatics s'utilitza el nom de la classe seguit de dos punts i el nom del metode
	// Per accedir a metodes no estatics s'utilitza el nom de l'objecte seguit de un punt i el nom del metode
	// La diferencia entre metodes estatics i no estatics es que els estatics no necessiten un objecte per ser cridats
	// Sino que es criden directament des del nom de la classe, no des del nom de l'objecte que has creat a partir de la classe.

	vec3 camera_pos = camera.getPos();
	double Cx = camera_pos[0];
	double Cy = camera_pos[1];
	double Cz = camera_pos[2];

	vec3 screen_center = camera.getScreenCenter();
	double Sx = screen_center[0];
	double Sy = screen_center[1];
	double Sz = screen_center[2];

	array<double, 4> pla_camera = camera.getScreenPlane();
	double A = pla_camera[0];
	double B = pla_camera[1];
	double C = pla_camera[2];
	double D = pla_camera[3];

	int pixelsX = camera.getScreenWidth();
	int pixelsY = camera.getScreenHeight();

	double numerador = -(A * Cx + B * Cy + C * Cz + D); // Per calcular la interseccion amb el pla de la camera

	double lenScreen_x = camera.getLenScreenX();
	double lenScreen_y = camera.getLenScreenY();

	double lenPixel_x = camera.getLenPixelX();
	double lenPixel_y = camera.getLenPixelY();

	vec3 Base_x = camera.getBaseX();
	vec3 Base_y = camera.getBaseY();

	vec3 inter_x;
	vec3 inter_y;
	vec3 inter_z;
	vec3 denominador;
	vec3 t;
	vec3 pant_x;
	vec3 pant_y;
	vec3 Vx;
	vec3 Vy;
	vec3 Vz;
	array<vec3, 3> v_direct;

	array<int, 3> pixel_coords_x;
	array<int, 3>  pixel_coords_y;

	for_each(execution::par, scene.triangles.begin(), scene.triangles.end(), [&](Triangle& triangle) {
		
		v_direct = triangle.vertexs;
		v_direct = v_direct - camera_pos;

		// Normalitzem columna a columna i extreiem les components
		v_direct[0] = unit_vector(v_direct[0]);
		v_direct[1] = unit_vector(v_direct[1]);
		v_direct[2] = unit_vector(v_direct[2]);

		Vx = getColumn(v_direct, 0);
		Vy = getColumn(v_direct, 1);
		Vz = getColumn(v_direct, 2);

		denominador = A * Vx + B * Vy + C * Vz;

		// cwiseInverse() es un metode de la classe Vector3d que retorna un vector columna amb els valors de l'inversa de cada element del vector
		t = numerador / denominador;

		// Per altra banda, cwizeProduct() es un metode de la classe Vector3d que retorna un vector columna amb els valors de la multiplicacio element a element dels dos vectors
		// Quan es fa un producte de dos vectors, per defecte es fa el producte escalar, pero si es vol fer el producte element a element s'ha d'utilitzar el metode cwiseProduct()

		inter_x = Cx + t * Vx;
		inter_y = Cy + t * Vy;
		inter_z = Cz + t * Vz;

		pant_x = (inter_x - Sx) * Base_x[0] + (inter_y - Sy) * Base_x[1] + (inter_z - Sz) * Base_x[2];
		pant_y = (inter_x - Sx) * Base_y[0] + (inter_y - Sy) * Base_y[1] + (inter_z - Sz) * Base_y[2];

		pant_x = pant_x + lenScreen_x / 2;
		pant_y = pant_y + lenScreen_y / 2;

		// Per poder fer el round() s'ha de convertir el vector en un array, ja que el metode round() no esta definit per vectors
		// Per convertir un vector en un array s'utilitza el metode array(), i per convertir un array en un vector s'utilitza el metode matrix()
		// Aixo es fa perque els vectors no tenen totes les funcions que tenen els arrays, i per tant, s'han de convertir en arrays per poder utilitzar aquestes funcions

		pixel_coords_x = (pant_x / lenPixel_x).round();
		pixel_coords_y = (pant_y / lenPixel_y).round();

		TraceLine(image, camera, pixel_coords_x[0], pixel_coords_y[0], pixel_coords_x[1], pixel_coords_y[1]);
		TraceLine(image, camera, pixel_coords_x[0], pixel_coords_y[0], pixel_coords_x[2], pixel_coords_y[2]);
		TraceLine(image, camera, pixel_coords_x[1], pixel_coords_y[1], pixel_coords_x[2], pixel_coords_y[2]);

		});

		

}
