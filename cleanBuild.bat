@echo off
setlocal

REM Set build directory and configuration
set BUILD_DIR=build
set CONFIG=Release

REM Delete old build directory to force fresh configuration
if exist "%BUILD_DIR%" (
    echo Deleting old build directory...
    rmdir /s /q "%BUILD_DIR%"
)

REM Configure project with vcpkg toolchain and prefix path
echo Configuring project...
cmake -B "%BUILD_DIR%" -S . ^
  -DCMAKE_TOOLCHAIN_FILE=vcpkg\scripts\buildsystems\vcpkg.cmake ^
  -DCMAKE_PREFIX_PATH=%CD%\vcpkg_installed\x64-windows

REM Build the project
echo Building project (%CONFIG%)...
cmake --build "%BUILD_DIR%" --config %CONFIG%

echo.
echo Build complete!
REM pause
