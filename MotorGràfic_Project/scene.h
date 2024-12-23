#pragma once
// Utilitzar vectors en comptes de llistes. Un vector es un array de tamany variable que es pot ampliar o reduir, i llegir de forma eficient.
// Es menys eficient que una llista en el cas de voler afegir o treure elements al mig de la llista, pero es mes eficient en la lectura.
// Ja que una llista pot tenir els elements en diferents posicions de la memoria, i un vector te els elements en posicions contigues.
// A mes accedir a un element d'un vector es immediat, mentre que per accedir a un element de la llista s'ha de recorrer la llista fins a la posicio de l'element.

// Per tal de poder incloure els triangles en el vector, definirem una estructura triangle que contindra tota la informacio del triangle.
// Farem el mateix amb la llum, definint una estructura llum que contindra tota la informacio de la llum.
// Les estructures poden contenir diferents tipus de variables, com ara vectors, doubles, ints, strings, etc.
// Els vectors nomes poden contenir elements del mateix tipus, a l'igual que les arrays, aixi que fent servir estructures podem tenir vectors amb elements iguals
// pero que continguin diferents tipus de variables.

# include <vector> 
# include <Eigen/Dense>

using namespace Eigen;

class scene
{
};

struct triangle
{
	Matrix<double, 3, 1> vertex1;
	Matrix<double, 3, 1> vertex2;
	Matrix<double, 3, 1> vertex3;
};

struct llum
{
	Matrix<double, 3, 1> posicio;
	double intensitat;
};