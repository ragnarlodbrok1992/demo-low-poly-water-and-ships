#ifndef _H_RENDER_DIRECTX3D12
#define _H_RENDER_DIRECTX3D12

#include <DirectXMath.h>
#include <cstdio>

struct Vertex {
  DirectX::XMFLOAT3 position;
  DirectX::XMFLOAT4 color;
};

const char* Vertex_to_string(const Vertex& vert);

#endif /* _H_RENDER_DIRECTX3D12 */

