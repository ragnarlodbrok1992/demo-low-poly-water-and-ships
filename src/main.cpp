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
static Microsoft::WRL::ComPtr<ID3D12Device> renderer_device;                                            
static Microsoft::WRL::ComPtr<ID3D12CommandQueue> renderer_commandQueue;

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
  create_device(renderer_useWarpDevice, renderer_device);
  create_command_queue(renderer_device, renderer_commandQueue);

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
