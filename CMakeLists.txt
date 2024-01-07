cmake_minimum_required (VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)

project(Muelsyse)

option(MUL_PROFILE "Enable Profile Test" OFF)
option(MUL_LOGGER "Enable Logger" ON)
option(MUL_DEBUG_ASSERT "Enable Assert" ON)
option(MUL_BUILD_SHARED "Build Dynamic Library" ON)
option(MUL_NO_CONSOLE "Build without console" OFF)

# MSVC Setting
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(
        "$<$<CONFIG:>:/MT>" #----------------|
        "$<$<CONFIG:Debug>:/MTd>" #----------|-- Statically link the runtime libraries
        "$<$<CONFIG:Release>:/MT>" #---------|
        "$<$<CONFIG:RelWithDebInfo>:/MT>" #--|
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(MUL_DEBUG)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/MP>")
endif()

# LTO error
# lto1.exe: error: two or more sections for ...
# (null):0: confused by earlier errors, bailing out
# lto-wrapper.exe: fatal error: ...

# if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#     add_compile_options(-flto)
# endif()

if(MUL_BUILD_SHARED)
    add_compile_definitions(MUL_BUILD_SHARED)
endif()

if(MUL_PROFILE)
    add_compile_definitions(MUL_PROFILE)
endif()

if(MUL_LOGGER)
    add_compile_definitions(MUL_LOGGER)
endif()

if(MUL_DEBUG_ASSERT)
    add_compile_definitions(MUL_DEBUG_ASSERT)
endif()

if(MUL_NO_CONSOLE)
    add_compile_definitions(MUL_NO_CONSOLE)
endif()

add_subdirectory(Muelsyse)
add_subdirectory(FlowingLab)
add_subdirectory(Flowing)