#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Engine variables
static const char* ClassName = "DemoLowPolySailing";
static const char* WindowName = "DemoLowPolySailing - alpha version.";
static const int   WindowWidth = 1280;
static const int   WindowHeight = 720;
static bool MainLoop = true;

// Key defines
#define VK_Q 0x51

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

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
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
