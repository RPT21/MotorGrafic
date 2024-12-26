#pragma once
#include <vector>
#include <Eigen/Dense>
#include "camera.h"
#include "scene.h"
using namespace Eigen;
using namespace std;

// Inline es una paraula clau que indica al compilador que el codi de la funcio es copii directament al lloc on es crida - Es posa abans de la definicio de la funcio
// S'utilitza quan es defineixen funcions en el header. Per solucionar aixo es millor declarar la funcio al header i definir la funcio al cpp corresponent.

void rasterizeImage(vector<unsigned char>& image, Camera& camera, Scene& scene);
void TraceLine(vector<unsigned char>& image, Camera& camera, int x0, int y0, int x1, int y1);



