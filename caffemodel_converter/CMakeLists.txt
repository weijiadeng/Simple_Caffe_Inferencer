cmake_minimum_required(VERSION 3.10)
project(model_converter)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-l protobuf")


add_executable(model_converter main.cpp external/caffe.pb.cpp external/caffe.pb.h main.h)