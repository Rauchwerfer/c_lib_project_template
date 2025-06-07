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

echo Running CMake with GCC...
%CMAKE% .. -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_C_COMPILER:FILEPATH=C:\msys64\ucrt64\bin\gcc.exe -DCMAKE_CXX_COMPILER:FILEPATH=C:\msys64\ucrt64\bin\g++.exe -G Ninja

echo Building the project...
cmake --build .

cd ..