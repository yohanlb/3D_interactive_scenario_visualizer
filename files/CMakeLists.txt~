cmake_minimum_required(VERSION 2.8)

project(3D_interactive_scenario_visualizer)

add_definitions(-std=c++11)
add_definitions(-Weffc++)
add_definitions(-Wall)
add_definitions(-O2)
add_definitions(-lassimp)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/third-party/CMake)

find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# Pour gérer un bug a la fac, a supprimer sur machine perso:
#set(OPENGL_LIBRARIES /usr/lib/x86_64-linux-gnu/libGL.so.1)

#assimp
include_directories(third-party/assimp/include)
set(ENABLE_BOOST_WORKAROUND ON CACHE BOOL "")
add_subdirectory(third-party/assimp)

include_directories(${SDL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR} engine/include glimac/include third-party/include)

set(ALL_LIBRARIES engine glimac assimp ${SDL_LIBRARY} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY})

add_subdirectory(glimac)
add_subdirectory(engine)

file(GLOB SOURCE_DIRECTORY "application")

add_subdirectory(${SOURCE_DIRECTORY})
