#pragma once
#include <vector>
#include <Eigen/Dense>
#include "camera.h"
#include "scene.h"
#include "vec3.h"
using namespace Eigen;
using namespace std;

// Inline es una paraula clau que indica al compilador que el codi de la funcio es copii directament al lloc on es crida - Es posa abans de la definicio de la funcio
// S'utilitza quan es defineixen funcions en el header. Per solucionar aixo es millor declarar la funcio al header i definir la funcio al cpp corresponent.

array<unsigned char, 3> TraceRay(Scene& scene, vec3 Point, vec3 RayDirection, const double& t_min, const double& t_max, const vector<array<vec3, 3>>& triangle_vertexs_matrix, const vector<array<double, 4>>& triangle_plane_matrix, const vector<array<array<double, 3>, 3>>& triangle_changeBaseMatrix, const int recursion_depth);
void raytraceImage(vector<unsigned char>& image, Camera& camera, Scene& scene);
int TriangleIntersection(const vec3& Point, const vec3& RayDirection, const double& t_min, const double& t_max, const vector<array<vec3, 3>>& triangle_vertexs_matrix, const vector<array<double, 4>>& triangle_plane_matrix, const vector<array<array<double, 3>, 3>>& M_inv, vec3& intersection);
