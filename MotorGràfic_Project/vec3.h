#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
#include <array>

using namespace std;

class vec3 {
public:
    // La classe v3 representa un array de 3 elements
    double e[3];

    // Constructors de la classe vec3 - Si no especifiquem, valen zero. Si posem un valor, s'assigna el valor. 
    // Per definir un vector amb components escriurem "vec3 nom_vector(Vx,Vy,Vz);"
    vec3() : e{ 0,0,0 } {}
    vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

    // Definim metodes per fer que el vector ens retorni les seves components - Exemple: "nom_vector.x();"
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(double t) {
        return *this *= 1 / t;
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
    array<int, 3>& round() {
        array<int, 3> returned_array;
        returned_array[0] = static_cast<int>(e[0]);
        returned_array[1] = static_cast<int>(e[1]);
        returned_array[2] = static_cast<int>(e[2]);
        return returned_array;
    }

};

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
using point3 = vec3;


// Vector Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}


inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator+(const double k, const vec3& v) {
    return vec3(k + v.e[0], k + v.e[1], k + v.e[2]);
}
inline vec3 operator+(const vec3& v, const double k) {
    return vec3(k + v.e[0], k + v.e[1], k + v.e[2]);
}

inline vec3 operator-(const double k, const vec3& v) {
    return vec3(k - v.e[0], k - v.e[1], k - v.e[2]);
}

inline vec3 operator-(const vec3& v, const double k) {
    return vec3(-k + v.e[0], -k + v.e[1], -k + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline array<vec3, 3> operator-(const array<vec3, 3>& Array, const vec3& v) {
    array<vec3, 3> return_array;
    return_array[0] = Array[0] - v;
    return_array[1] = Array[1] - v;
    return_array[2] = Array[2] - v;

    return return_array;
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
    return (1 / t) * v;
}
inline vec3 operator/(double t, const vec3& v) {
    vec3 return_v;
    return_v[0] = 1 / v.e[0];
    return_v[1] = 1 / v.e[1];
    return_v[2] = 1 / v.e[2];

    return t * return_v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2];
}
inline vec3 matrix_dot_vector(const array<vec3, 3>& Matrix, const vec3& v) {
    vec3 returned_vector;
    returned_vector[0] = dot(Matrix[0], v);
    returned_vector[1] = dot(Matrix[1], v);
    returned_vector[2] = dot(Matrix[2], v);
    return returned_vector;

}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

inline vec3 getColumn(array<vec3, 3> Matrix, int c) {
    vec3 return_vector;
    return_vector[0] = Matrix[0][c];
    return_vector[1] = Matrix[1][c];
    return_vector[2] = Matrix[2][c];
    return return_vector;
}

#endif

