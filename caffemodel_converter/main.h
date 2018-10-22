//
// Created by dwj on 9/12/18.
//

#ifndef MODEL_CONVERTER_MAIN_H
#define MODEL_CONVERTER_MAIN_H

#include <string>
#include <vector>

#define FP16 0
#define FP32 1
#define FP64 2


struct TestAppArgs
{
    std::string input_name;
    std::string output_folder;
    std::string precision;
    int precision_code;
    bool show_help;
    TestAppArgs() :
            input_name(""),
            output_folder("./"),
            show_help(false),
            precision("fp32"),
            precision_code(0)
    {}
};

///Test whether a folder exists.
int is_folder_exist(const std::string & path);

///Output the weight data from caffemodel
bool get_parameters_form_caffemodel(std::string const &model_path, std::string const &out_path);

std::string num_to_string(int num);


#endif //MODEL_CONVERTER_MAIN_H
