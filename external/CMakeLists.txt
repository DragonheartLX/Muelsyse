## GLFW
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw)

## glm
add_subdirectory(glm)

## stb_image
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE stb)

## glad
add_library(glad INTERFACE)
target_sources(glad INTERFACE ./glad/glad.c)
target_include_directories(glad INTERFACE glad)

## spdlog
add_subdirectory(spdlog)