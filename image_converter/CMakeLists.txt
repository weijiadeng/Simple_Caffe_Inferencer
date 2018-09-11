cmake_minimum_required(VERSION 3.10)
project(image_converter)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-l jpeg")


add_executable(image_converter main.cpp interface.h Image.cpp Image.h interface.cpp main.h)