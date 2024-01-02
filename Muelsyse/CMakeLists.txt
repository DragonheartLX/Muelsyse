file(GLOB_RECURSE MLS_SRC LIST_DIRECTORIES false 
    *.cpp
    **/*.cpp)

project(MuelsyseLib)

if(MUL_BUILD_TYPE STREQUAL "Static")
    add_library(MuelsyseLib STATIC ${MLS_SRC})
elseif(MUL_BUILD_TYPE STREQUAL "Shared")
    add_compile_definitions(MUL_BUILD_DLL)
    add_library(MuelsyseLib SHARED ${MLS_SRC})
endif()

target_include_directories(MuelsyseLib PUBLIC .)
target_precompile_headers(MuelsyseLib PRIVATE mulpch.h)

find_package(glad CONFIG REQUIRED)
    target_link_libraries(MuelsyseLib PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
    target_link_libraries(MuelsyseLib PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
    target_link_libraries(MuelsyseLib PRIVATE glm::glm)

find_package(spdlog CONFIG REQUIRED)
    target_link_libraries(MuelsyseLib PUBLIC spdlog::spdlog_header_only)

find_package(Stb REQUIRED)
    target_include_directories(MuelsyseLib PRIVATE ${Stb_INCLUDE_DIR})

if(MUL_BUILD_TYPE STREQUAL "Static")
    install(TARGETS MuelsyseLib DESTINATION lib)
elseif(MUL_BUILD_TYPE STREQUAL "Shared")
    install(TARGETS MuelsyseLib DESTINATION bin)
endif()