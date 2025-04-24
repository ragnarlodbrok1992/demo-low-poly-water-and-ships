#ifndef _H_RENDER_DIRECTX3D12
#define _H_RENDER_DIRECTX3D12

#define _WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <DirectXMath.h>
#include <cstdio>
#include <stdexcept>

#include "directx/d3dcommon.h"
#include "directx/d3d12.h"
#include "directx/d3dx12.h"

#include <dxgi1_6.h>

// Internal defines
#define FRAME_COUNT 2

// Vertex stuff
struct Vertex {
  DirectX::XMFLOAT3 position;
  DirectX::XMFLOAT4 color;
};

const char* Vertex_to_string(const Vertex& vert);

// Renderer initialization functions
void create_device(
    Microsoft::WRL::ComPtr<IDXGIFactory4>& factory,
    Microsoft::WRL::ComPtr<ID3D12Device>& device,
    bool useWarpDevice);
void create_command_queue(
    Microsoft::WRL::ComPtr<ID3D12Device>& device,
    Microsoft::WRL::ComPtr<ID3D12CommandQueue>& commandQueue);
void create_swap_chain(
    Microsoft::WRL::ComPtr<IDXGIFactory4>& factory,
    Microsoft::WRL::ComPtr<ID3D12CommandQueue>& commandQueue,
    Microsoft::WRL::ComPtr<IDXGISwapChain3>& r_swapChain,
    const int m_width,
    const int m_height,
    const HWND hwnd,
    UINT64 frame_index);
void create_descriptor_heaps_frame_resource(
    Microsoft::WRL::ComPtr<ID3D12Device>& device,
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap,
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& commandAllocator,
    Microsoft::WRL::ComPtr<IDXGISwapChain3>& r_swapChain,
    Microsoft::WRL::ComPtr<ID3D12Resource> renderTargets[FRAME_COUNT], // Passing as pointer, since arrays get coerced to pointers, not by reference, possible source of bug... Also this really opens my eye to directx3d initialization procedure, pushing it down through functions might not be such a good idea...
    UINT& rtvDescriptorSize);
// void create_frame_resources(); // Part of descriptor_heap
void create_empty_root_signature_pipeline_state();

// Helper functions
inline void ThrowIfFailed(HRESULT hr); 
void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter);

#endif /* _H_RENDER_DIRECTX3D12 */

