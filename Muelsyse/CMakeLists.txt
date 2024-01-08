file(GLOB_RECURSE MLS_SRC LIST_DIRECTORIES false 
    *.cpp
    **/*.cpp)

project(MuelsyseLib)

if(MUL_OPT_BUILD_SHARED)
    add_compile_definitions(GLAD_GLAPI_EXPORT_BUILD)
    add_compile_definitions(MUL_BUILD_DLL)
    add_library(MuelsyseLib SHARED ${MLS_SRC})
else()
    add_library(MuelsyseLib STATIC ${MLS_SRC})
endif()

target_include_directories(MuelsyseLib PUBLIC .)

target_precompile_headers(MuelsyseLib PRIVATE mulpch.h)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set_property(TARGET MuelsyseLib APPEND PROPERTY LINK_FLAGS "/NODEFAULTLIB:MSVCRTD")
endif()

find_package(glfw3 CONFIG REQUIRED)
    target_link_libraries(MuelsyseLib PUBLIC glfw)

find_package(glm CONFIG REQUIRED)
    target_link_libraries(MuelsyseLib PRIVATE glm::glm)

find_package(spdlog CONFIG REQUIRED)
    target_link_libraries(MuelsyseLib PUBLIC spdlog::spdlog_header_only)

find_package(Stb REQUIRED)
    target_include_directories(MuelsyseLib PRIVATE ${Stb_INCLUDE_DIR})

if(MUL_OPT_BUILD_SHARED)
    install(TARGETS MuelsyseLib DESTINATION bin)
else()
    install(TARGETS MuelsyseLib DESTINATION lib)
endif()