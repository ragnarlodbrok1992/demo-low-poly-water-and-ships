#ifndef _H_RENDER_DIRECTX3D12
#define _H_RENDER_DIRECTX3D12

#define _WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <DirectXMath.h>
#include <dxgi1_6.h>
#include <cstdio>
#include <stdexcept>

#include "directx/d3dcommon.h"
#include "directx/d3d12.h"
#include "directx/d3dx12.h"

// Vertex stuff
struct Vertex {
  DirectX::XMFLOAT3 position;
  DirectX::XMFLOAT4 color;
};

const char* Vertex_to_string(const Vertex& vert);

// Renderer initialization functions
void create_device(bool useWarpDevice, Microsoft::WRL::ComPtr<ID3D12Device>& device);
void create_command_queue(
    Microsoft::WRL::ComPtr<ID3D12Device>& device,
    Microsoft::WRL::ComPtr<ID3D12CommandQueue>& commandQueue);
void create_swap_chain();
void create_window_association();
void create_descriptor_heaps();
void create_frame_resources();

// Helper functions
inline void ThrowIfFailed(HRESULT hr); 
void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter);

#endif /* _H_RENDER_DIRECTX3D12 */

