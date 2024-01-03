cmake_minimum_required (VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)

project(Muelsyse)

# MSVC设置: 静态库设置
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(
        "$<$<CONFIG:>:/MT>" #---------|
        "$<$<CONFIG:Debug>:/MTd>" #---|-- Statically link the runtime libraries
        "$<$<CONFIG:Release>:/MT>" #--|
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(MUL_DEBUG)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(MUL_RELEASE)
endif()

if(MUL_BUILD_TYPE STREQUAL "Static")
    add_compile_definitions(MUL_BUILD_STATIC)
endif()

if(MUL_BUILD_TYPE STREQUAL "Shared")
    add_compile_definitions(GLAD_GLAPI_EXPORT)
    add_compile_definitions(MUL_BUILD_SHARED)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_definitions(MUL_MSVC)
    add_compile_definitions(MUL_PLATFORM_WINDOWS)
endif()

add_subdirectory(Muelsyse)
add_subdirectory(FlowingLab)
add_subdirectory(Flowing)