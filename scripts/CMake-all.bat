@echo off

pushd %~dp0\..\
echo ----- Create Build Folder -----
md build
cd build
popd

echo ----- Show CMake preset -----
cmake .. --list-presets

echo ----- Generate CMake preset -----
echo ----- Generate MSVC x64 Static Debug preset -----
cmake .. --preset msvc-x64-static-debug

echo ----- Generate MSVC x64 Static Debug preset -----
cmake .. --preset msvc-x64-static-release

echo ----- Generate MSVC x64 Static Debug preset -----
cmake .. --preset msvc-x64-static-RelWithDebInfo

echo ----- Generate MSVC x64 Static Debug preset -----
cmake .. --preset msvc-x64-dynamic-debug

echo ----- Generate MSVC x64 Static Debug preset -----
cmake .. --preset msvc-x64-dynamic-release

echo ----- Generate MSVC x64 Static Debug preset -----
cmake .. --preset msvc-x64-dynamic-RelWithDebInfo

echo ----- Generate MSVC x64 Static Debug preset -----
cmake .. --preset mingw-x64-static-debug

echo ----- Generate MSVC x64 Static Debug preset -----
cmake .. --preset mingw-x64-static-release

echo ----- Generate MSVC x64 Static Debug preset -----
cmake .. --preset mingw-x64-static-RelWithDebInfo

echo ----- Generate MSVC x64 Static Debug preset -----
cmake .. --preset mingw-x64-dynamic-debug

echo ----- Generate MSVC x64 Static Debug preset -----
cmake .. --preset mingw-x64-dynamic-release

echo ----- Generate MSVC x64 Static Debug preset -----
cmake .. --preset mingw-x64-dynamic-RelWithDebInfo

pushd %~dp0\..\
echo ----- Build MSVC x64 Static Debug preset -----
cmake --build --preset MSVC-x64-static-debug

echo ----- Build MSVC x64 Static Release preset -----
cmake --build --preset MSVC-x64-static-release

echo ----- Build MSVC x64 Static RelWithDebInfo preset -----
cmake --build --preset MSVC-x64-static-RelWithDebInfo

echo ----- Build MSVC x64 Dynamic Debug preset -----
cmake --build --preset MSVC-x64-dynamic-debug

echo ----- Build MSVC x64 Dynamic Release preset -----
cmake --build --preset MSVC-x64-dynamic-release

echo ----- Build MSVC x64 Dynamic RelWithDebInfo preset -----
cmake --build --preset MSVC-x64-dynamic-RelWithDebInfo

echo ----- Build MinGW x64 Static Debug preset -----
cmake --build --preset MinGW-x64-static-debug

echo ----- Build MinGW x64 Static Release preset -----
cmake --build --preset MinGW-x64-static-release

echo ----- Build MinGW x64 Static RelWithDebInfo preset -----
cmake --build --preset MinGW-x64-static-RelWithDebInfo

echo ----- Build MinGW x64 Dynamic Debug preset -----
cmake --build --preset MinGW-x64-dynamic-debug

echo ----- Build MinGW x64 Dynamic Release preset -----
cmake --build --preset MinGW-x64-dynamic-release

echo ----- Build MinGW x64 Dynamic RelWithDebInfo preset -----
cmake --build --preset MinGW-x64-dynamic-RelWithDebInfo
popd

echo ----- Build Finshed -----
PAUSE