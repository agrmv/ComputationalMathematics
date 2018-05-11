cmake_minimum_required(VERSION 3.9)
project(GaussSeidel)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES main.cpp header.h)
add_executable(GaussSeidel main.cpp header.h)