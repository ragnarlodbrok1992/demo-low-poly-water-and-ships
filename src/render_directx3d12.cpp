#include "render_directx3d12.hpp"

const char* Vertex_to_string(const Vertex& vert) {
  static char buffer[256];
  snprintf(buffer, sizeof(buffer), "Vertex --> position: %.2f, %.2f, %.2f; color: %.2f %.2f %.2f %.2f", vert.position.x, vert.position.y, vert.position.z, vert.color.x, vert.color.y, vert.color.z, vert.color.w);

  return buffer;
}

void create_device(
    Microsoft::WRL::ComPtr<IDXGIFactory4>& factory,
    Microsoft::WRL::ComPtr<ID3D12Device>& device,
    bool useWarpDevice) {
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

void create_swap_chain(
    Microsoft::WRL::ComPtr<IDXGIFactory4>& factory,
    Microsoft::WRL::ComPtr<ID3D12CommandQueue>& commandQueue,
    Microsoft::WRL::ComPtr<IDXGISwapChain3>& r_swapChain,
    const int m_width,
    const int m_height,
    const HWND hwnd,
    UINT64 frame_index) {
  DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
  swapChainDesc.BufferCount          = FRAME_COUNT;
  swapChainDesc.BufferDesc.Width     = m_width;
  swapChainDesc.BufferDesc.Height    = m_height;
  swapChainDesc.BufferDesc.Format    = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferUsage          = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.SwapEffect           = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  swapChainDesc.OutputWindow         = hwnd;
  swapChainDesc.SampleDesc.Count     = 1;
  swapChainDesc.Windowed             = TRUE;

  Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
  ThrowIfFailed(factory->CreateSwapChain(
        commandQueue.Get(),
        &swapChainDesc,
        &swapChain
    ));

  ThrowIfFailed(swapChain.As(&r_swapChain));

  // Initialization - setting up fullscreen
  ThrowIfFailed(factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));
  frame_index = r_swapChain->GetCurrentBackBufferIndex();
}

void create_descriptor_heaps_frame_resource(
    Microsoft::WRL::ComPtr<ID3D12Device>& device,
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap,
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& commandAllocator,
    Microsoft::WRL::ComPtr<IDXGISwapChain3>& r_swapChain,
    Microsoft::WRL::ComPtr<ID3D12Resource> renderTargets[FRAME_COUNT],
    UINT& rtvDescriptorSize) {

    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors             = FRAME_COUNT;
    rtvHeapDesc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    ThrowIfFailed(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap)));

    rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

  // Initialization - frame resources
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart());   

    // RTV for each frame
    for (UINT n = 0; n < FRAME_COUNT; n++) {
      ThrowIfFailed(r_swapChain->GetBuffer(n, IID_PPV_ARGS(&renderTargets[n])));
      device->CreateRenderTargetView(renderTargets[n].Get(), nullptr, rtvHandle);
      rtvHandle.Offset(1, rtvDescriptorSize);
    }

  ThrowIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)));
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
