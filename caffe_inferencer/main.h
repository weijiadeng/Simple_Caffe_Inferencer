//
// Created by dwj on 8/30/18.
//
// This file reference structure information from the nvdla/sw project
// https://github.com/nvdla/sw
// Created by Deng Weijia on 7/20/18.
//



#ifndef CAFFE_INFERENCER_MAIN_H
#define CAFFE_INFERENCER_MAIN_H



#include <string>



struct TestAppArgs
{
    std::string model_path;
    std::string proto_name;
    std::string input_image;
    std::string output_path;
    bool drop_middle;
    bool show_help;

    TestAppArgs() :
            model_path(""),
            proto_name(""),
            input_image(""),
            output_path("./"),
            drop_middle(false),
            show_help(false)

    {}
};



//Test whether a folder exists.
int is_folder_exist(const std::string & path);





#endif //CAFFE_INFERENCER_MAIN_H
