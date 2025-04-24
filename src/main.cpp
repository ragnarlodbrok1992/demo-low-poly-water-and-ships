#include "demo_low_poly_sailing.h"
#include "render_directx3d12.hpp"

// Building defines
#define DEBUG

// Engine variables
static const char* ClassName = "DemoLowPolySailing";
static const char* WindowName = "DemoLowPolySailing - alpha version.";
static const int   WindowWidth = 1280;
static const int   WindowHeight = 720;
static float renderer_aspectRatio = static_cast<float>(WindowWidth) / static_cast<float>(WindowHeight);
static bool MainLoop = true;

// Renderer objects
static bool renderer_useWarpDevice = false; // WARP - Windows Advanced Rasterization Platform - software renderer
static UINT64 frame_index;
static UINT rtv_descriptor_size;
static Microsoft::WRL::ComPtr<IDXGIFactory4> renderer_factory;
static Microsoft::WRL::ComPtr<ID3D12Device> renderer_device;                                            
static Microsoft::WRL::ComPtr<ID3D12CommandAllocator> renderer_commandAllocator;
static Microsoft::WRL::ComPtr<ID3D12CommandQueue> renderer_commandQueue;
static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> renderer_rtvHeap;
static Microsoft::WRL::ComPtr<IDXGISwapChain3> renderer_swapChain;
static Microsoft::WRL::ComPtr<ID3D12Resource> renderer_renderTargets[FRAME_COUNT];

// Key defines
#define VK_Q 0x51

// Test stuff for directx3d12 rendering system
static Vertex vert = {};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  // Destroying application
  switch (message) {
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
      }
      break;
    case WM_KEYDOWN: {
                       // Quit on Q key
        if (wParam == VK_Q) { // VK_Q
          // PostQuitMessage(0);
          MainLoop = false;
          return 0;
        }
      }
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
  // Just to not have warning here
  return DefWindowProc(hWnd, message, wParam, lParam);
}

// Logger metadata
const char* LOGGER_FILE_NAME = "logger.log";
std::ofstream LOGGER_STREAM;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  // Test stuff here
  // Setting up logger
  Vertex test_vert = {};
  register_logger_file(LOGGER_FILE_NAME, LOGGER_STREAM);
  log("First main message.");
  log(Vertex_to_string(test_vert));

  // Creating window class object
  WNDCLASSEX wc = {
    sizeof(WNDCLASSEX),
    CS_HREDRAW|CS_VREDRAW,
    WndProc,
    0,
    0,
    hInstance,
    0,
    0,
    0,
    0,
    ClassName,
    0
  };

  RegisterClassEx(&wc);

  HWND hwnd = CreateWindow(
    ClassName,
    WindowName,
    WS_OVERLAPPEDWINDOW,
    100,
    100,
    WindowWidth,
    WindowHeight,
    nullptr,
    nullptr,
    hInstance,
    nullptr
      );

  ShowWindow(hwnd, nCmdShow);

  // After properly starting window - prepare directx stuff
#if defined(DEBUG)
  {
      Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
      if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
      {
        debugController->EnableDebugLayer();
      }
  }
#endif

  // Create functions from renderer
  create_device(renderer_factory, renderer_device, renderer_useWarpDevice);
  create_command_queue(renderer_device, renderer_commandQueue);
  create_swap_chain(renderer_factory, renderer_commandQueue, renderer_swapChain, WindowWidth, WindowHeight, hwnd, frame_index);
  create_descriptor_heaps_frame_resource(renderer_device, renderer_rtvHeap, renderer_commandAllocator, renderer_swapChain, renderer_renderTargets, rtv_descriptor_size);

  // Main loop
  MSG msg = {};

  while (MainLoop) {
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int) msg.wParam;
}
