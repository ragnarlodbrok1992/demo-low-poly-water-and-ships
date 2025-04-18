#ifndef _H_RENDER_DIRECTX3D12
#define _H_RENDER_DIRECTX3D12

#include <DirectXMath.h>

struct Vertex {
  DirectX::XMFLOAT3 position;
  DirectX::XMFLOAT4 color;

  friend std::ostream& operator<<(std::ostream& os, const Vertex& vert) {
    os << "Test debug vert stream stuff.";
    return os;
  }
};

#endif /* _H_RENDER_DIRECTX3D12 */

