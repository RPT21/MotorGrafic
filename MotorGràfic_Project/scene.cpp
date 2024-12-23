#include "scene.h"

Triangle::Triangle(const Vector3d& v1, const Vector3d& v2, const Vector3d& v3, const Matrix<unsigned char, 3, 1>& c, const float s, const float r)
{
	vertexs.col(0) = v1;
	vertexs.col(1) = v2;
	vertexs.col(2) = v3; 
	color = c;
	specular = s;
	reflectance = r;
}
