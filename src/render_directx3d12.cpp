#include "render_directx3d12.hpp"

const char* Vertex_to_string(const Vertex& vert) {
  static char buffer[256];
  snprintf(buffer, sizeof(buffer), "Vertex --> position: %.2f, %.2f, %.2f; color: %.2f %.2f %.2f %.2f", vert.position.x, vert.position.y, vert.position.z, vert.color.x, vert.color.y, vert.color.z, vert.color.w);

  return buffer;
};

