cmake_minimum_required(VERSION 3.11)
project(CursWork)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
add_executable(CursWork main.cpp Interpolation.h Grid.h)
target_link_libraries(CursWork ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )