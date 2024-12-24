#pragma once
#include <vector>
#include <Eigen/Dense>
#include "camera.h"
#include "scene.h"
using namespace Eigen;
using namespace std;

void rasterizeImage(vector<unsigned char>& image, Camera& camera, Scene& scene) {

	size_t number_triangles = scene.triangles.size();

	Vector3d camera_pos = camera.getPos();
		double Cx = camera_pos[0];
		double Cy = camera_pos[1];
		double Cz = camera_pos[2];

	Vector4d pla_camera = camera.getPlane();
		double A = pla_camera[0];
		double B = pla_camera[1];
		double C = pla_camera[2];
		double D = pla_camera[3];

	for (int i = 0; i < number_triangles; i++) {

		Matrix3d v_direct = scene.triangles[i].vertexs.colwise() - camera_pos;
		// Normalitzem columna a columna i exteiem les components
			v_direct = v_direct.colwise().normalized();
				Vector3d Vx = v_direct.row(0);
				Vector3d Vy = v_direct.row(1);
				Vector3d Vz = v_direct.row(2);

		double numerador = -(A * Cx + B * Cy + C * Cz + D);
		Vector3d denominador = A * Vx + B * Vy + C * Vz;

        Vector3d t = numerador * denominador.cwiseInverse();

		Vector3d inter_x = Vector3d::Constant(Cx) + t.cwiseProduct(Vx);
		Vector3d inter_y = Vector3d::Constant(Cy) + t.cwiseProduct(Vy);
		Vector3d inter_z = Vector3d::Constant(Cz) + t.cwiseProduct(Vz);

		// Mirar que significa Constant...

	}
}

