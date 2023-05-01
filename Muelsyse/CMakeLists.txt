file(GLOB_RECURSE MLS_SRC LIST_DIRECTORIES false 
    *.cpp
    **/*.cpp)

add_library(Muelsyse STATIC ${MLS_SRC})

target_include_directories(Muelsyse PUBLIC .)

target_link_libraries(Muelsyse PUBLIC
    glfw
    glm::glm
    stb
    glad
    spdlog::spdlog_header_only)

install(TARGETS Muelsyse DESTINATION bin)