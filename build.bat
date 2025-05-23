@echo off

set BUILD_DIR="build"
set COMPILER="D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.43.34808\bin\Hostx64\x64\cl.exe"

set DIRECTX_HEADERS_INCLUDE_DIR="..\external\directx-headers\include"
set DIRECTX_HEADERS_LIB_DIR="..\external\directx-headers\"

set ASSETS_FOLDER="..\src\assets"

echo Checking for build directory...

IF NOT EXIST %BUILD_DIR% (
  echo Creating build directory...
  mkdir "%BUILD_DIR%"
)

pushd %BUILD_DIR%

echo Entering build directory..

REM echo Copying assets folder...

REM xcopy "%ASSETS_FOLDER%" "." /E /H /C /I /Y

echo Compiling...

%COMPILER% /EHsc /Zi^
  /DEBUG:FULL^
  /INCREMENT:NO^
  ../src/main.cpp^
  ../src/logger.cpp^
  ../src/render_directx3d12.cpp^
  /I%DIRECTX_HEADERS_INCLUDE_DIR%^
  user32.lib gdi32.lib d3d12.lib dxgi.lib d3dcompiler.lib^
  /link /subsystem:windows /out:DemoLowPolySailing.exe

popd

