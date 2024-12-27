#include "raytracer.h"
#include <execution>
#include <algorithm>
#include <vector>
#include <limits>
#include <cmath>
#include <numeric>
#include <array>
#include "vec3.h"

vec3 Eigen_to_Vec(const Vector3d& Vector) {
	vec3 result;
	for (int n = 0; n < 3; n++) {
		result[n] = Vector[n];
	}
	return result;
}

void raytraceImage(vector<unsigned char>& image, Camera& camera, Scene& scene)
{
	static bool initialized = false;
	static vector<array<double, 5>> light_matrix;
	static vector<array<vec3, 3>> triangle_vertexs_matrix;
	static vector<array<array<double, 3>, 3>> triangle_changeBaseMatrix;
	static vector<array<double, 4>> triangle_plane_matrix;
	static vector<array<unsigned char, 3>> triangle_color_matrix;

	if (!initialized) {
		for_each(scene.triangles.begin(), scene.triangles.end(), [&](Triangle& triangle) {
			triangle_vertexs_matrix.emplace_back(triangle.vertexs);
			triangle_changeBaseMatrix.emplace_back(triangle.changeBaseMatrix);
			triangle_plane_matrix.emplace_back(triangle.pla_triangle);
			triangle_color_matrix.emplace_back(triangle.color);
		});
		for_each(scene.lights.begin(), scene.lights.end(), [&](Light& light) {
			array<double, 5> light_row;
			if (light.type == POINT) {
				light_row[0] = light.position[0];
				light_row[1] = light.position[1];
				light_row[2] = light.position[2];
				light_row[3] = 0;
			}
			else if (light.type == DIRECTIONAL) {
				light_row[0] = light.direction[0];
				light_row[1] = light.direction[1];
				light_row[2] = light.direction[2];
				light_row[3] = 1;
			}
			else {
				light_row[0] = 0;
				light_row[1] = 0;
				light_row[2] = 0;
				light_row[3] = 2;
			}
			light_row[4] = light.intensity;
			light_matrix.emplace_back(light_row);
			});

		initialized = true;

	}

	static int pixelsX = camera.getScreenWidth();
	static int pixelsY = camera.getScreenHeight();

	vec3 camera_pos = camera.getPos();

	double Cx = camera_pos[0];
	double Cy = camera_pos[1];
	double Cz = camera_pos[2];

	vec3 screen_center = camera.getScreenCenter();
	double Sx = screen_center[0];
	double Sy = screen_center[1];
	double Sz = screen_center[2];

	static double lenScreen_x = camera.getLenScreenX();
	static double lenScreen_y = camera.getLenScreenY();

	static double lenPixel_x = camera.getLenPixelX();
	static double lenPixel_y = camera.getLenPixelY();

	static double epsilon = 0.0001;

	vec3 Base_x = camera.getBaseX();
	vec3 Base_y = camera.getBaseY();

	vec3 pantalla_init;

	pantalla_init = screen_center - (lenScreen_x / 2) * Base_x - (lenScreen_y / 2) * Base_y;

	vector<int> px_indices(pixelsX);
	iota(px_indices.begin(), px_indices.end(), 0);
	for_each(execution::par, px_indices.begin(), px_indices.end(), [&](int Px) {

		vec3 pixelPos;
		vec3 light_direction;
		vec3 light_direct_norm;
		vec3 Reflected_Ray;
		vec3 triangle_norm;
		vec3 RayDirection;

		vec3 inter_array;
		vec3 inter_array_L;

		array<unsigned char, 3> triangle_color;

		double pixel_intensity = 1;
		double dot_prod_triangle_camera;
		double dot_prod_triangle_light;
		double dot_prod_Reflect_Camera;
		int nearest_triangle;
		int nearest_triangle_L;
		double specular;

		bool isCameraFrontside;
		bool isLightFrontside;

		for(int Py = 0; Py < pixelsY; Py++) {

			pixelPos = pantalla_init + Px * lenPixel_x * Base_x + Py * lenPixel_y * Base_y;
			RayDirection = pixelPos - camera_pos;

			nearest_triangle = TraceRayIntersection(camera_pos, RayDirection, 0, numeric_limits<double>::max(), triangle_vertexs_matrix, triangle_plane_matrix, triangle_changeBaseMatrix, inter_array);
			
			if (nearest_triangle != -1) {
				
				triangle_norm = scene.triangles[nearest_triangle].norm_vector;

				RayDirection = unit_vector(RayDirection);

				// Ho fem negatiu perque mirem el vector des del triangle cap a la camera
				dot_prod_triangle_camera = - dot(triangle_norm, RayDirection);

				if (dot_prod_triangle_camera >= 0) {
					isCameraFrontside = true;
				}
				else {
					isCameraFrontside = false;
				}

				pixel_intensity = 0;

				for_each(scene.lights.begin(), scene.lights.end(), [&](Light& light) {
					if (light.type == POINT or light.type == DIRECTIONAL) {

						if (light.type == POINT){
						light_direction = light.position - inter_array;
						}
						else {
							// Mirem la direccio de la llum des del triangle
							light_direction = -light.direction;
						}

						light_direct_norm = unit_vector(light_direction);
						dot_prod_triangle_light = dot(light_direct_norm, triangle_norm);

						if (dot_prod_triangle_light >= 0) {
							isLightFrontside = true;
						}
						else {
							isLightFrontside = false;
						}

						if (isCameraFrontside == isLightFrontside) {
							if (light.type == POINT) {
								nearest_triangle_L = TraceRayIntersection(inter_array, light_direction, epsilon, 1, triangle_vertexs_matrix, triangle_plane_matrix, triangle_changeBaseMatrix, inter_array_L);
							}
							else {
								nearest_triangle_L = TraceRayIntersection(inter_array, light_direction, epsilon, numeric_limits<double>::max(), triangle_vertexs_matrix, triangle_plane_matrix, triangle_changeBaseMatrix, inter_array_L);
							}			

							if (nearest_triangle_L == -1) {
								pixel_intensity += light.intensity * abs(dot_prod_triangle_light);
								specular = scene.triangles[nearest_triangle].specular;
								if (specular != -1) {
									Reflected_Ray = 2 * triangle_norm * dot_prod_triangle_light - light_direction;
									Reflected_Ray = unit_vector(Reflected_Ray);

									// Posem signe negatiu perque el vector surt del triangle i va cap a la camera
									dot_prod_Reflect_Camera = -dot(Reflected_Ray, RayDirection);
									pixel_intensity += light.intensity * pow(abs(dot_prod_Reflect_Camera), specular);

								}

							}
						}
					}
					else {
						// LLum ambiental
						pixel_intensity += light.intensity;
					}
				});

				if (pixel_intensity > 1)
					pixel_intensity = 1;
				
				triangle_color = scene.triangles[nearest_triangle].color;

				image[3 * (Px + Py * pixelsX)] = static_cast<unsigned char>(pixel_intensity * triangle_color[0]);
				image[3 * (Px + Py * pixelsX) + 1] = static_cast<unsigned char>(pixel_intensity * triangle_color[1]);
				image[3 * (Px + Py * pixelsX) + 2] = static_cast<unsigned char>(pixel_intensity * triangle_color[2]);

			}
			else {
				image[3 * (Px + Py * pixelsX)] = 255;
				image[3 * (Px + Py * pixelsX) + 1] = 255;
				image[3 * (Px + Py * pixelsX) + 2] = 255;
			}
			
		}
	});
	

}

int TraceRayIntersection(const vec3& Point, const vec3& RayDirection, const double& t_min, const double& t_max, const vector<array<vec3, 3>>& triangle_vertexs_matrix, const vector<array<double, 4>>& triangle_plane_matrix, const vector<array<array<double, 3>, 3>>& M_inv, vec3& intersection)
{
	size_t number_triangles = triangle_plane_matrix.size();
	double numerador;
	double denominador;
	double t;
	double inter_x;
	double inter_y;
	double inter_z;
	double alfa;
	double beta;
	double A, B, C, D;

	double t_intersect = numeric_limits<double>::max();
	int nearest_triangle = -1;

	for (int n = 0; n < number_triangles; n++) {

		A = triangle_plane_matrix[n][0];
		B = triangle_plane_matrix[n][1];
		C = triangle_plane_matrix[n][2];
		D = triangle_plane_matrix[n][3];

		numerador = -(A * Point[0] + B * Point[1] + C * Point[2] + D);
		denominador = A * RayDirection[0] + B * RayDirection[1] + C * RayDirection[2];

		if (denominador != 0){

			t = numerador / denominador;

			inter_x = Point[0] + t * RayDirection[0];
			inter_y = Point[1] + t * RayDirection[1];
			inter_z = Point[2] + t * RayDirection[2];

			alfa = (inter_x - triangle_vertexs_matrix[n][0][0]) * M_inv[n][0][0] + (inter_y - triangle_vertexs_matrix[n][0][1]) * M_inv[n][0][1] + (inter_z - triangle_vertexs_matrix[n][0][2]) * M_inv[n][0][2];
			beta = (inter_x - triangle_vertexs_matrix[n][0][0]) * M_inv[n][1][0] + (inter_y - triangle_vertexs_matrix[n][0][1]) * M_inv[n][1][1] + (inter_z - triangle_vertexs_matrix[n][0][2]) * M_inv[n][1][2];

			if (alfa >= 0 && beta >= 0 && alfa + beta <= 1) {
				if (t > t_min && t < t_max) {
					if (t < t_intersect) {
						t_intersect = t;
						nearest_triangle = n;
						intersection = { inter_x, inter_y, inter_z };
					}
				}
			}

		}
	}
	return nearest_triangle;
}

