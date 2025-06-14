@echo off
cls
set BUILD_DIR=build
set CMAKE="E:\Programs\CMake\bin\cmake.exe"

REM Debug or Release there
set CONFIG=Release

echo Cleaning previous build...
rmdir /S /Q %BUILD_DIR%

echo Creating build directory...
mkdir %BUILD_DIR%
cd %BUILD_DIR%

echo Running CMake with MSVC (Visual Studio 2022)...
%CMAKE% .. -G "Visual Studio 17 2022" -A x64

echo Building the project...
%CMAKE% --build . --config %CONFIG% --verbose

cd ..