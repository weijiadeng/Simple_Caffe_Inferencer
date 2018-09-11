cmake_minimum_required(VERSION 3.10)
project(model_converter)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-l protobuf")

#set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -Wno-unused-but-set-variable")

set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -Wno-unused-but-set-variable")


add_executable(model_converter main.cpp external/caffe.pb.cpp external/caffe.pb.h main.h)
