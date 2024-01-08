# Muelsyse Flowing CMake file
file(GLOB_RECURSE FLOWING_SRC LIST_DIRECTORIES false 
    *.cpp
    **/*.cpp)

# Project Name
project(Flowing)

if(MUL_OPT_NO_CONSOLE)
    if(MSVC)
        add_executable(${PROJECT_NAME} ${FLOWING_SRC})
        target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:WINDOWS")
    else()
        add_executable(${PROJECT_NAME} WIN32 ${FLOWING_SRC})
    endif()
else()
    add_executable(${PROJECT_NAME} ${FLOWING_SRC})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE .)

target_link_libraries(${PROJECT_NAME} PRIVATE MuelsyseLib)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)