cmake_minimum_required (VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)

project(Muelsyse)

option(MUL_OPT_PROFILE "Enable Profile Test" OFF)
option(MUL_OPT_LOGGER "Enable Logger" ON)
option(MUL_OPT_DEBUG_ASSERT "Enable Assert" ON)
option(MUL_OPT_BUILD_SHARED "Build Dynamic Library" ON)
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

# Clang Setting
if(CMAKE_CXX_COMPILER_ID STREQUAL "clang")
    add_compile_options(
        "$<$<CONFIG:>:-fms-runtime-lib=static>" #-------------------|
        "$<$<CONFIG:Debug>:-fms-runtime-lib=static_dbg>" #----------|-- Statically link the runtime libraries
        "$<$<CONFIG:Release>:-fms-runtime-lib=static>" #------------|
        "$<$<CONFIG:RelWithDebInfo>:-fms-runtime-lib=static>" #-----|
    )
endif()

message("-- [Muelsyse] Build Type: ${CMAKE_BUILD_TYPE}")
message("-- [Muelsyse] C Compiler: ${CMAKE_C_COMPILER_ID}")
message("-- [Muelsyse] CXX Compiler: ${CMAKE_CXX_COMPILER_ID}")

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

add_subdirectory(Muelsyse)
add_subdirectory(FlowingLab)
add_subdirectory(Flowing)