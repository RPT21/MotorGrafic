#include "scene.h"

Triangle::Triangle(const vec3& v1, const vec3& v2, const vec3& v3, const array<unsigned char, 3>& c, const float s, const float r)
{
	vertexs[0] = v1;
	vertexs[1] = v2;
	vertexs[2] = v3;
	color = c;
	specular = s;
	reflectance = r;
	
	vec3 v1v2 = v2 - v1;
	vec3 v1v3 = v3 - v1;

	vec3 normal = cross(v1v3, v1v2);
	normal = unit_vector(normal);
	norm_vector = normal;

	double D = -normal[0] * v1[0] - normal[1] * v1[1] - normal[2] * v1[2];

	pla_triangle[0] = normal[0];
	pla_triangle[1] = normal[1];
	pla_triangle[2] = normal[2];
	pla_triangle[3] = D;

	Matrix3d M;

	for (int i = 0; i < 3; i++) {
		M(i, 0) = v1v2[i];
	}

	for (int i = 0; i < 3; i++) {
		M(i, 1) = v1v3[i];
	}

	for (int i = 0; i < 3; i++) {
		M(i, 2) = normal[i];
	}
		
	M = M.inverse().eval();

	// Es necessita utilitzar eval() per a que es faci la inversa de la matriu
	// La funcio inverse() retorna una matriu amb l'inversa de la matriu original
	// Pero canvia la matriu original a la vegada que retorna la inversa
	// eval() fa una copia de la matriu original i retorna la inversa de la copia

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				changeBaseMatrix[i][j] = M(i, j);
			}
	}
	
}
