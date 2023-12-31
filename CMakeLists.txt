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

add_subdirectory(Muelsyse)
add_subdirectory(FlowingLab)
add_subdirectory(Flowing)