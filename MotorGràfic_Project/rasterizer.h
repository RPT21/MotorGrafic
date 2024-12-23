#pragma once
#include <vector>
#include <Eigen/Dense>
#include "camera.h"
#include "scene.h"
using namespace Eigen;
using namespace std;

void rasterizeImage(vector<unsigned char>& image, Camera& camera, Scene& scene) {
	int number_triangles = scene.triangles.size();
	for (int i = 0; i < number_triangles; i++) {
		Matrix3d v_direct = scene.triangles[i].vertexs - camera.getPos();
	}
}

