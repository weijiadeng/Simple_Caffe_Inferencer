
//
// Created by dwj on 8/31/18.
//


///Parse model files.


#pragma once
#include<fstream>
#include<iostream>
#include<sstream>
#include<vector>
#include<string>
#include<boost/algorithm/string.hpp>
#include<sys/types.h>
#include<dirent.h>
#include<cstdio>
#include<cerrno>
#include"tensor.h"
#include"layer_decoder.h"
#include"tools.h"
#include "global_val.h"




///Get a list of all possible filenames
bool get_filename(const std::string & target, std::vector<std::string>& file_list, std::vector<Layer> const & layer_list);

///Convert a sentence to a list of numbers.
template<typename T>
std::vector<T> string_to_array(std::string const & source) {
    std::vector<std::string> target_list;
    split(target_list, source, boost::is_any_of(" "), boost::token_compress_on);
    std::vector<T> result(target_list.size());
    for (uint i = 0; i < target_list.size(); i++)
    {
        result[i] = string_to_num<T>(target_list[i]);
    }
    return result;
}

///Read one model file
bool read_model_file(std::string const & path, std::vector<std::vector<Tensor*>>& model_list, Layer_decoder const & layer_decoder) ;

///Read the whole model folder
bool read_model(std::string const & path, std::vector<std::vector<Tensor *> > & model_list, Layer_decoder const & ld, std::vector<Layer> layer_list);

///Read the data file, which has slightly different format from model file
bool read_data_file(std::string const & path, std::vector<Tensor *>  & data_list);


///Read a single tensor. A model file may contain may tensor.
bool read_tensor(std::string & str, std::istream & ifs,std::vector<Tensor *> & data_list);


