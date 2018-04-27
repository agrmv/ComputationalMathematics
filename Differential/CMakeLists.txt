cmake_minimum_required(VERSION 3.11)
project(Differential)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
add_executable(Differential main.cpp Grid.h)
target_link_libraries(Differential ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )