cmake_minimum_required(VERSION 3.11)
project(Interpolation)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
add_executable(Lagrange main.cpp Interpolation.h Glut.h)
target_link_libraries(Lagrange ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )