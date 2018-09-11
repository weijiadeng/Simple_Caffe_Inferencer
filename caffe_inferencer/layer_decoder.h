//
// Created by dwj on 8/31/18.
//


///Parse layer configuration from prototxt class

#pragma once
#include<vector>
#include<unordered_map>
#include"prototxt_parser.h"
#include"layer.h"
#include "global_val.h"





struct Layer_decoder{
    ///Calculation type dict
    std::unordered_map<std::string, Layer::Parameter::Cal_type> cal_type_dict;

    ///Translate the name to the ordinal number of the layer
    std::unordered_map<std::string, unsigned> number_dict;
    ///Assign the index of the output and input value.
    void assign_values(unsigned &number, Prototxt const &x, std::string const &item,
                       std::vector<unsigned int> &col);
    ///Assign parameters for the layers
    void assign_params(Layer & ly, Prototxt const & x);
    ///Decode prototxt from given file.
    bool decode_prototxt(std::vector<Prototxt> const &input, std::vector<Layer> &output);
};





