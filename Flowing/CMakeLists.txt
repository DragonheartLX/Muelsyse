# Muelsyse Application CMake file
file(GLOB_RECURSE FLOWING_SRC LIST_DIRECTORIES false 
    *.cpp
    **/*.cpp)

project(Flowing)

add_executable(Flowing ${FLOWING_SRC})

# No console in Release
# if(MSVC)
#     add_executable(Flowing ${FLOWING_SRC})
# 
#     if(CMAKE_BUILD_TYPE MATCHES "Debug")
#         target_link_options(Flowing PRIVATE "/entry:WinMainCRTStartup")
#     endif()
# 
#     if(CMAKE_BUILD_TYPE MATCHES "Release")
#         target_link_options(Flowing PRIVATE "/SUBSYSTEM:WINDOWS")
#     endif()
# else()
#     if(CMAKE_BUILD_TYPE MATCHES "Debug")
#         add_executable(Flowing ${FLOWING_SRC})
#     endif()
#     
#     if(CMAKE_BUILD_TYPE MATCHES "Release")
#         add_executable(Flowing WIN32 ${FLOWING_SRC})
#     endif()
# endif()

target_include_directories(Flowing PRIVATE .)

target_link_libraries(Flowing PRIVATE MuelsyseLib)

install(TARGETS Flowing DESTINATION bin)