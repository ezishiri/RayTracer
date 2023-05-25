#include "geometry.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

bool scene_intersect(const Vec3f &orig, const Vec3f &dir,
                     const std::vector<Sphere> &spheres, Vec3f &hit, Vec3f &N,
                     Material &material) {
  float spheres_dist = std::numeric_limits<float>::max();
  for (size_t i = 0; i < spheres.size(); i++) {
    float dist_i;
    if (spheres[i].ray_intersect(orig, dir, dist_i) && dist_i < spheres_dist) {
      spheres_dist = dist_i;
      hit = orig + dir * dist_i;
      N = (hit - spheres[i].center).normalize();
      material = spheres[i].material;
    }
  }
  return spheres_dist < 1000;
}

Vec3f cast_ray(const Vec3f &orig, const Vec3f &dir,
               const std::vector<Sphere> &spheres,
               const std::vector<Light> &lights) {
  Vec3f point, N;
  Material material;

  if (!scene_intersect(orig, dir, spheres, point, N, material)) {
    return Vec3f(0.2, 0.3, 0.2); // background color
  }
  float diffuse_light_intensity = 0;
  for (size_t i = 0; i < lights.size(); i++) {
    Vec3f light_dir = (lights[i].position - point).normalize();
    diffuse_light_intensity +=
        lights[i].intensity * std::max(0.f, light_dir.dot(N));
  }
  return material.diffuse_color * diffuse_light_intensity;
}

void render(const std::vector<Sphere> &spheres,
            const std::vector<Light> &lights) {
  const int width = 2048;
  const int height = 1560;
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
      framebuffer[i + j * width] =
          cast_ray(Vec3f(0, 0, 0), dir, spheres, lights);
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
  srand(time(NULL));

  std::vector<Sphere> spheres;
  // generate twenty spheres
  const int num_spheres = 20; // number of spheres to add
  const float spacing = 3.0;  // spacing between spheres

  for (int i = 0; i < num_spheres; i++) {
    float x = rand() % 20 - 10; // random x coordinate between -10 and 10
    float y = rand() % 20 - 10; // random y coordinate between -10 and 10
    float z = rand() % 10 - 20; // random z coordinate between -20 and -10
    float x_color = (float)rand() / (float)RAND_MAX;
    float y_color = (float)rand() / (float)RAND_MAX;
    float z_color = (float)rand() / (float)RAND_MAX;
    x_color = (float)((int)(x_color * 100)) /
              100; // limit precision to 2 decimal places
    y_color = (float)((int)(y_color * 100)) / 100;
    z_color = (float)((int)(z_color * 100)) / 100;
    float radius = 2;
    Material color(Vec3f(x_color, y_color, z_color));
    spheres.push_back(Sphere(Vec3f(x, y, z), radius, color));
  }

  std::vector<Light> lights;
  lights.push_back(Light(Vec3f(-20, 20, 20), 1.5));

  render(spheres, lights);
  int message[] = {119, 104, 97,  116, 32,  100, 111, 32,  121, 111, 117,
                   32,  116, 104, 105, 110, 107, 32,  97,  98,  111, 117,
                   116, 32,  109, 121, 32,  116, 105, 110, 121, 32,  114,
                   101, 110, 100, 101, 114, 101, 114, 33,  32};

  for (int i = 0; i < sizeof(message) / sizeof(message[0]); i++) {
    cout << (char)message[i];
  }
  return 0;
}
