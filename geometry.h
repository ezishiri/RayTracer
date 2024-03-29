#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

// for future improvements I could probably use a template for all the diff
// types of vectors it's possible to work with (combinations of floats, doubles,
// ints, etc)

struct Vec3f {
  float x, y, z; // declare our three floats
  Vec3f() {      // default constructor
    x = 0;
    y = 0;
    z = 0;
  }
  Vec3f(float a, float b, float c) { // user input
    x = a;
    y = b;
    z = c;
  }

  float operator[](size_t index) {
    if (index >= 3) {
      cout << "Array index out of bound, exiting";
      exit(0);
    }
    if (index == 0) {
      return x;
    } else if (index == 1) {
      return y;
    } else if (index == 2) {
      return z;
    } else {
      std::cout << " Index out of bounds, exiting";
      exit(0);
    }
  }

  // operator overloading for printing out values of 3d float vectors
  friend std::ostream &operator<<(std::ostream &os, const Vec3f &v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
  }

  // below simple operator overloadings for addition, subtraction,
  // cross product, dot product, and various simple vector
  // operations
  Vec3f operator+(const Vec3f &v) const {
    return Vec3f(x + v.x, y + v.y, z + v.z);
  }

  Vec3f operator+=(const Vec3f &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  Vec3f operator-(const Vec3f &v) const {
    return Vec3f(x - v.x, y - v.y, z - v.z);
  }

  Vec3f operator-=(const Vec3f &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  // for my purposes, * is scalar multiplication
  Vec3f operator*(const float s) const { return Vec3f(s * x, s * y, s * z); }

  Vec3f operator*=(const float s) {
    x *= s;
    y *= s;
    z *= s;
    return *this;
  }

  // dot product
  float dot(const Vec3f &v) { return x * v.x + y * v.y + z * v.z; }

  // cross product
  Vec3f cross(const Vec3f &v) {
    float i = y * v.z - z * v.y;
    float j = z * v.x - x * v.z;
    float k = x * v.y - y * v.x;

    return Vec3f(i, j, k);
  }

  Vec3f normalize() {
    float magnitude = sqrt(x * x + y * y + z * z);
    return Vec3f(x / magnitude, y / magnitude, z / magnitude);
  }
};

// now repeat for 2d float vectors
struct Vec2f {
  float x, y; // declare our two floats
  Vec2f() {   // default constructor
    x = 0;
    y = 0;
  }
  Vec2f(float a, float b) { // user input
    x = a;
    y = b;
  }

  // operator overloading for printing out values of 2d vectors
  friend std::ostream &operator<<(std::ostream &os, const Vec2f &v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
  }
};

struct Light {
    Light(const Vec3f &p, const float &i) : position(p), intensity(i) {}
    Vec3f position;
    float intensity;
};

struct Material {
    Material(const Vec3f &color) : diffuse_color(color) {}
    Material() : diffuse_color() {}
    Vec3f diffuse_color;
};


struct Sphere {
  Vec3f center;
  float radius;
  Material material;

  Sphere(const Vec3f &c, const float &r, const Material &m) : center(c), radius(r), material(m) {}
  bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0) const {
    Vec3f L = center - orig;
    float tca = L.dot(dir);
    float d2 = L.dot(L) - tca * tca;
    if (d2 > radius * radius)
      return false;
    float thc = sqrtf(radius * radius - d2);
    t0 = tca - thc;
    float t1 = tca + thc;
    if (t0 < 0)
      t0 = t1;
    if (t0 < 0)
      return false;
    return true;
  }
};

#endif //__GEOMETRY_H__
