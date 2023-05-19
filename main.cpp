#include "geometry.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

Vec3f cast_ray(const Vec3f &orig, const Vec3f &dir, const Sphere &sphere) {
  float sphere_dist = std::numeric_limits<float>::max();
  if (!sphere.ray_intersect(orig, dir, sphere_dist)) {
    return Vec3f(0.2, 0.7, 0.8); // color 1
  }
  return Vec3f(0.4, 0.4, 0.3); // color 2
}

void render(const Sphere &sphere) {
  const int width = 1024;
  const int height = 768;
  const float fov = M_PI / 2.;
  std::vector<Vec3f> framebuffer(width *
                                 height); // array of 3d vectors (rgb vals)
  // this is [(r1,g1,b1), (r2,g2,b2), (r3,g3,b3), (r4,g4,b4)...] for every pixel
  // in our frame
  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) * width /
                (float)height;
      float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
      Vec3f dir = Vec3f(x, y, -1).normalize();
      framebuffer[i + j * width] = cast_ray(Vec3f(0, 0, 0), dir, sphere);
    }
  }

  // now write an image to disk in ppm format
  // later will include stb library so we can use other formats like jpeg, png,
  // etc
  std::ofstream ofs;     // save the framebuffer to file
  ofs.open("./out.ppm"); // this is from tinyraytracer
  ofs << "P6\n" << width << " " << height << "\n255\n";
  for (size_t i = 0; i < height * width; ++i) {
    for (size_t j = 0; j < 3; j++) {
      ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
    }
  }
  ofs.close();
}

int main() {
  Sphere sphere1(Vec3f(-3, 0, -16), 2);
  // Sphere sphere2(Vec3f(3, 0, 16), 2);
  // Sphere sphere3(Vec3f(-10, 8, 4), 5);
  // Sphere sphere4(Vec3f(4, 5, 6), 8);
  // Sphere sphere5(Vec3f(11, -10, 3), 2);
  render(sphere1);
  // render(sphere2);
  // render(sphere3);
  // render(sphere4);
  // render(sphere5);

  return 0;
}
