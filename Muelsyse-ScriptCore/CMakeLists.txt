cmake_minimum_required(VERSION 3.19)

set(LIBRARY_OUTPUT_PATH "${MUL_ROOT_PATH}/FlowingTest/Resources/Scripts")
message("-- [ScriptCore] Output Path: ${LIBRARY_OUTPUT_PATH}")

file(GLOB_RECURSE MUL_CORE_SRC LIST_DIRECTORIES false 
    *.cs
    **/*.cs)

project(Muelsyse-ScriptCore LANGUAGES CSharp)

add_library(Muelsyse-ScriptCore SHARED ${MUL_CORE_SRC})

set_property(TARGET Muelsyse-ScriptCore PROPERTY DOTNET_TARGET_FRAMEWORK_VERSION "v4.7.2")

set_property(TARGET Muelsyse-ScriptCore PROPERTY VS_DOTNET_REFERENCES
    "Microsoft.CSharp"
    "System"
)
