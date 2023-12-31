# Muelsyse Editor CMake file
file(GLOB_RECURSE FLOWING_LAB_SRC LIST_DIRECTORIES false 
    *.cpp
    **/*.cpp)

project(FlowingLab)

add_executable(FlowingLab ${FLOWING_LAB_SRC})

# No console in Release
# if(MSVC)
#     add_executable(FlowingLab ${FLOWING_LAB_SRC})
# 
#     if(CMAKE_BUILD_TYPE MATCHES "Debug")
#         target_link_options(FlowingLab PRIVATE "/entry:WinMainCRTStartup")
#     endif()
# 
#     if(CMAKE_BUILD_TYPE MATCHES "Release")
#         target_link_options(FlowingLab PRIVATE "/SUBSYSTEM:WINDOWS")
#     endif()
# else()
#     if(CMAKE_BUILD_TYPE MATCHES "Debug")
#         add_executable(FlowingLab ${FLOWING_LAB_SRC})
#     endif()
#     
#     if(CMAKE_BUILD_TYPE MATCHES "Release")
#         add_executable(FlowingLab WIN32 ${FLOWING_LAB_SRC})
#     endif()
# endif()

target_include_directories(FlowingLab PRIVATE .)

target_link_libraries(FlowingLab PRIVATE Muelsyse)

install(TARGETS FlowingLab DESTINATION bin)