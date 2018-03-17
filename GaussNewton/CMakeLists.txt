cmake_minimum_required(VERSION 3.9)
project(gaussnewton)

set(CMAKE_CXX_STANDARD 17)
set(DERECTORY /home/agrmv/learning/ComputationalMathematics/GaussSeidel/cmake-build-debug)
add_library( GaussSeidel STATIC IMPORTED )
set_property( TARGET GaussSeidel PROPERTY IMPORTED_LOCATION ${DERECTORY}/libGaussSeidel.a)
add_executable(gaussnewton main.cpp header.h)