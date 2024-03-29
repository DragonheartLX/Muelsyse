cmake_minimum_required (VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)

# Default vcpkg triplet
set(VCPKG_TARGET_TRIPLET "x64-win-llvm-static")
# -DVCPKG_TARGET_TRIPLET=x64-win-llvm-static

# Use Clang-CL (default)
set(CMAKE_GENERATOR_TOOLSET "ClangCL")

project(Muelsyse)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/bin>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib>)
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")
set(MUL_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

message("-- [Muelsyse] EXECUTABLE_OUTPUT_PATH: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message("-- [Muelsyse] LIBRARY_OUTPUT_PATH (Static): ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message("-- [Muelsyse] LIBRARY_OUTPUT_PATH (Shared): ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message("-- [Muelsyse] CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")

option(MUL_OPT_PROFILE "Enable Profile Test" OFF)
option(MUL_OPT_LOGGER "Enable Logger" ON)
option(MUL_OPT_DEBUG_ASSERT "Enable Assert" ON)
option(MUL_OPT_BUILD_SHARED "Build Dynamic Library" OFF)
option(MUL_OPT_NO_CONSOLE "Build without console" OFF)

# MSVC Setting
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(
        "$<$<CONFIG:>:/MT>" #----------------|
        "$<$<CONFIG:Debug>:/MTd>" #----------|-- Statically link the runtime libraries
        "$<$<CONFIG:Release>:/MT>" #---------|
        "$<$<CONFIG:RelWithDebInfo>:/MT>" #--|
    )
endif()

# Clang-cl Setting
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
        add_compile_options(
            "$<$<CONFIG:>:-fms-runtime-lib=static>" #-------------------|
            "$<$<CONFIG:Debug>:-fms-runtime-lib=static_dbg>" #----------|-- Statically link the runtime libraries
            "$<$<CONFIG:Release>:-fms-runtime-lib=static>" #------------|
            "$<$<CONFIG:RelWithDebInfo>:-fms-runtime-lib=static>" #-----|
        )
    endif()
endif()

message("-- [Muelsyse] C Compiler: ${CMAKE_C_COMPILER_ID}")
message("-- [Muelsyse] CXX Compiler: ${CMAKE_CXX_COMPILER_ID}")

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/MP>")
endif()

if(MUL_OPT_BUILD_SHARED)
    message("-- [Muelsyse] Enable Shared Library")
    add_compile_definitions(MUL_BUILD_SHARED)
endif()

if(MUL_OPT_PROFILE)
    message("-- [Muelsyse] Enable Profile")
    add_compile_definitions(MUL_PROFILE)
endif()

if(MUL_OPT_LOGGER)
    message("-- [Muelsyse] Enable Logger")
    add_compile_definitions(MUL_LOGGER)
endif()

if(MUL_OPT_DEBUG_ASSERT)
    message("-- [Muelsyse] Enable Debug Assert")
    add_compile_definitions(MUL_DEBUG_ASSERT)
endif()

if(MUL_OPT_NO_CONSOLE)
    message("-- [Muelsyse] Disable Console Window")
    add_compile_definitions(MUL_NO_CONSOLE)
endif()

set(MONO_LIB_LIST 
    ${CMAKE_CURRENT_SOURCE_DIR}/External/mono/Release/libmono-static-sgen.lib
    Ws2_32.lib
    Winmm.lib
    Version.lib
    Bcrypt.lib
)

add_subdirectory(Muelsyse)
add_subdirectory(FlowingLab)
add_subdirectory(Flowing)
add_subdirectory(Muelsyse-ScriptCore)
add_subdirectory(FlowingTest/Project)