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

// Anem a explicar perque s'utilitza la paraula const en els arguments de les funcions.
// Si un argument es passa per referencia, es a dir, es passa la direccio de memoria de la variable, aquest argument es pot modificar dins de la funcio, i modificara la variable original.
// Si es vol que l'argument no es pugui modificar dins de la funcio, s'ha de posar la paraula const davant de l'argument.
// Aixo es molt util per evitar errors, ja que si es modifica un argument per referencia, es modifica la variable original externa a la funcio, i aixo pot provocar errors dificils de trobar.
// A mes, si es posa const, el compilador pot optimitzar el codi, ja que sap que l'argument no es modificara dins de la funcio.
// Els arguments de les funcions en aquest cas son temporals, ja que no definim cap variable, sino que passem el valor directament a la funcio per carregar el triangle o la llum.
// Una funcio no pot acceptar la referencia a una variable temporal que no sigui const, perque no es possible modificar una variable temporal.
// Per tant, si es vol passar una variable temporal per referencia, s'ha de posar const davant de l'argument.
// La variable temporal es crea al cridar la funcio, i es destrueix al finalitzar la funcio, per tant no es pot modificar.

// Un avantatge de passar per referencia es que no es fa una copia de la variable, sino que es passa la direccio de memoria de la variable.
// Aixo es molt util quan es treballa amb estructures de dades grans, ja que es molt mes eficient passar la direccio de memoria que no copiar tota la estructura.
// Aixi estem llegint i escrivint a la mateixa posicio de memoria, i no estem fent copies innecessaries de dades.
// Perque quan passem un argument a una funcio, es crea una copia de la variable, i aixo pot ser molt lent si la variable ocupa molta memoria.

# include <vector> 
# include <Eigen/Dense>

using namespace Eigen;
using namespace std;

enum LightType {
	POINT = 0,
	DIRECTIONAL = 1,
	AMBIENT = 2
};

struct Triangle
{
	Matrix3d vertexs;
	Matrix<unsigned char, 3, 1> color;
	float specular;
	float reflectance;

	// Definim el constructor de la estructura Triangle:
	// Assignem els valors dels vertexs, el color, la brillantor i la reflectancia al crear el triangle.
	// Els vertexs estan definits en una matriu de 3x3, on cada columna es un vertex.
	Triangle(const Vector3d& v1, const Vector3d& v2, const Vector3d& v3, const Matrix<unsigned char, 3, 1>& c, const float s, const float r);
 
};

struct Light
{
	LightType type;
	Matrix<double, 3, 1> position;
	Matrix<double, 3, 1> direction;
	Matrix<unsigned char, 3, 1> color;
	float intensity;

	// Definim el constructor de la estructura Light
	// Assignem els valors del tipus de llum, la posicio, la direccio, el color i la intensitat al crear la llum
	Light(const LightType& t, const Vector3d& pos, const Vector3d& dir, const Matrix<unsigned char, 3, 1>& col, const float& inten)
		: type(t), position(pos), direction(dir), color(col), intensity(inten) {}
	
};

struct Scene
{
	vector<Triangle> triangles; // Vector de triangles
	vector<Light> lights; // Vector de llums

	// La funcio push_back fa una copia de l'element i l'afegeix al final del vector. L'element ha de ser previament creat, no es pot afegir directament al vector.
	// En canvi, emplace_back es una funcio que afegeix un element al final del vector, el construeix directament al final del vector, i et demana els arguments per construir l'element.
	// En el nostre cas tenim un vector que conte estructures, els elements del vector son estructures.
	// La funcio inicialitza l'estructura amb els arguments que se li passen al final del vector.

	void addTriangle(const Vector3d& v1, const Vector3d& v2, const Vector3d& v3, const Matrix<unsigned char, 3, 1> color, const float specular, const float reflectance) {
		triangles.emplace_back(v1, v2, v3, color, specular, reflectance); // Crea i afegix directament el triangle
	}
	void addLight(const LightType type, const Vector3d& position, const Vector3d& direction, const Matrix<unsigned char, 3, 1> color, const float intensity) {
		lights.emplace_back(type, position, direction, color, intensity); // Crea i afegix directament la llum
	}
};
