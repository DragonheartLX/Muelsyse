# Muelsyse Application CMake file
file(GLOB_RECURSE MLS_SRC LIST_DIRECTORIES false 
    *.cpp
    **/*.cpp)

add_executable(MuelsyseBox ${MLS_SRC})

# No console in Release
# if(MSVC)
#     add_executable(MuelsyseBox ${MLS_SRC})
# 
#     if(CMAKE_BUILD_TYPE MATCHES "Debug")
#         target_link_options(MuelsyseBox PRIVATE "/entry:WinMainCRTStartup")
#     endif()
# 
#     if(CMAKE_BUILD_TYPE MATCHES "Release")
#         target_link_options(MuelsyseBox PRIVATE "/SUBSYSTEM:WINDOWS")
#     endif()
# else()
#     if(CMAKE_BUILD_TYPE MATCHES "Debug")
#         add_executable(MuelsyseBox ${MLS_SRC})
#     endif()
#     
#     if(CMAKE_BUILD_TYPE MATCHES "Release")
#         add_executable(MuelsyseBox WIN32 ${MLS_SRC})
#     endif()
# endif()

target_include_directories(MuelsyseBox PRIVATE .)

target_link_libraries(MuelsyseBox PRIVATE Muelsyse)

install(TARGETS MuelsyseBox DESTINATION bin)