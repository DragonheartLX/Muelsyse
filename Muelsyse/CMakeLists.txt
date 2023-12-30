file(GLOB_RECURSE MLS_SRC LIST_DIRECTORIES false 
    *.cpp
    **/*.cpp)

project(MuelsyseLib)

add_library(Muelsyse STATIC ${MLS_SRC})

target_include_directories(Muelsyse PUBLIC .)

find_package(glad CONFIG REQUIRED)
    target_link_libraries(Muelsyse PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
    target_link_libraries(Muelsyse PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
    target_link_libraries(Muelsyse PRIVATE glm::glm)

find_package(spdlog CONFIG REQUIRED)
    target_link_libraries(Muelsyse PRIVATE spdlog::spdlog_header_only)

find_package(Stb REQUIRED)
    target_include_directories(Muelsyse PRIVATE ${Stb_INCLUDE_DIR})

install(TARGETS Muelsyse DESTINATION lib)