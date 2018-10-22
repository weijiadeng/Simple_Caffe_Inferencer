cmake_minimum_required(VERSION 3.10)
project(caffe_inferencer)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -Wno-unused-but-set-variable")

set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -Wno-unused-but-set-variable")


add_executable(caffe_inferencer main.cpp layer.h global_val.h prototxt_parser.h prototxt_parser.cpp layer_cal.h layer_decoder.cpp layer_decoder.h model_parser.h model_parser.cpp tensor.h tools.cpp tools.h driver.cpp driver.h output.cpp output.h main.h tokenizer.cpp tokenizer.h layers/conv.h layers/general.h layers/pooling.h layers/softmax.h layers/relu.h layers/eltwise.h layers/bn.h layers/scale.h layers/fc.h external/half.h)