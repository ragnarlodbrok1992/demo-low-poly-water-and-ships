#include "render_directx3d12.hpp"

const char* Vertex_to_string(const Vertex& vert) {
  static char buffer[256];
  snprintf(buffer, sizeof(buffer), "Vertex --> position: %.2f, %.2f, %.2f; color: %.2f %.2f %.2f %.2f", vert.position.x, vert.position.y, vert.position.z, vert.color.x, vert.color.y, vert.color.z, vert.color.w);

  return buffer;
}

void create_device(bool useWarpDevice, Microsoft::WRL::ComPtr<ID3D12Device>& device) {
  Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
  ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

  if (useWarpDevice) {
    // TODO: what will happen if we use software renderer?
  } else {
    Microsoft::WRL::ComPtr<IDXGIAdapter1> hardwareAdapter;
    GetHardwareAdapter(factory.Get(), &hardwareAdapter);

    ThrowIfFailed(D3D12CreateDevice(
      hardwareAdapter.Get(),
      D3D_FEATURE_LEVEL_11_0,
      IID_PPV_ARGS(&device)
          ));
  }
}

void create_command_queue(
    Microsoft::WRL::ComPtr<ID3D12Device>& device,
    Microsoft::WRL::ComPtr<ID3D12CommandQueue>& commandQueue) {
  D3D12_COMMAND_QUEUE_DESC queueDesc = {};
  queueDesc.Flags                    = D3D12_COMMAND_QUEUE_FLAG_NONE;
  queueDesc.Type                     = D3D12_COMMAND_LIST_TYPE_DIRECT;

  ThrowIfFailed(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue)));
}

inline void ThrowIfFailed(HRESULT hr) {
  if (FAILED(hr)) {
    throw std::runtime_error("Failed HRESULT");
  }
}

void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter) {
  *ppAdapter = nullptr;
  for (UINT adapterIndex = 0; ; ++adapterIndex) {
      IDXGIAdapter1* pAdapter = nullptr;
      if (DXGI_ERROR_NOT_FOUND == pFactory->EnumAdapters1(adapterIndex, &pAdapter)) {
          // No more adapters to enumerate.
          break;
      } 

      // Check to see if the adapter supports Direct3D 12, but don't create the
      // actual device yet.
      if (SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
          *ppAdapter = pAdapter;
          return;
      }
      pAdapter->Release();
  }
}
