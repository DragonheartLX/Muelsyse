cmake_minimum_required(VERSION 3.19)

set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Cache/Binaries")
message("-- [FlowingCSharp] Output Path: ${LIBRARY_OUTPUT_PATH}")

file(GLOB_RECURSE FLOWING_SRC LIST_DIRECTORIES false 
    Assets/*.cs
    Assets/**/*.cs)

project(FlowingCSharp LANGUAGES CSharp)

add_library(FlowingCSharp SHARED ${FLOWING_SRC})

set_property(TARGET FlowingCSharp PROPERTY DOTNET_TARGET_FRAMEWORK_VERSION "v4.7.2")

set_property(TARGET FlowingCSharp PROPERTY VS_DOTNET_REFERENCES
    "Microsoft.CSharp"
    "System"
)

target_link_libraries(FlowingCSharp PRIVATE Muelsyse-ScriptCore)