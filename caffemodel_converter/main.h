//
// Created by dwj on 9/12/18.
//

#ifndef MODEL_CONVERTER_MAIN_H
#define MODEL_CONVERTER_MAIN_H

#include <string>
#include <vector>




struct TestAppArgs
{
    std::string input_name;
    std::string output_folder;
    bool show_help;
    TestAppArgs() :
            input_name(""),
            output_folder(""),
            show_help(false)
    {}
};

///Test whether a folder exists.
int is_folder_exist(const std::string & path);

///Output the weight data from caffemodel
bool get_parameters_form_caffemodel(std::string const &model_path, std::string const &out_path);

std::string num_to_string(int num);


#endif //MODEL_CONVERTER_MAIN_H
