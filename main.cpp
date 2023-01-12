#include "geometry.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

void render() {
  const int width = 1024;
  const int height = 768;
  std::vector<Vec3f> framebuffer(width * height); // array of 3d vectors (rgb vals) 
  // this is [(r1,g1,b1), (r2,g2,b2), (r3,g3,b3), (r4,g4,b4)...] for every pixel in our frame 

  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      framebuffer[i + j * width] =
          Vec3f(j / float(height), i / float(width), 1);
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
  render();

  return 0;
}
