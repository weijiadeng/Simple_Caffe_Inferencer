//
// Created by deng weijia on 8/30/18.
//
/**
* This file manages the calculation process.It receives the layer information,iterates 
* through all layers, calls corresponding layer calculation method with required model 
* data and tensor data, and handles post layer calculation processes such as copy data
* to corresponding targets.
* The calculation process is straight forward, namely process layers in the exact order
* which is defined in the prototxt file.Any cases that need extra process are supposed 
* to be done by the layer_decoder process.
*/
#pragma once

#include<vector>
#include"layer.h"
#include"layer_cal.h"
#include "tensor.h"
#include "output.h"
#include "tools.h"



///Manages post calculation process for each layer, including copying calculated  data to
///target address, output debug information and call the method of writing data files.
bool cal_post_process(Layer const & ly, Tensor * result, std::vector<Tensor *> & data_list, int layer_index, std::string const & output_path, bool is_output);

///Reads the layer info of one layer and calls the corresponding layer calculation method.
bool handle_layer(Layer const & ly, int layer_index, std::vector<std::vector<Tensor *> > const & param_list, std::vector<Tensor *> & data_list, std::string const & output_path, bool is_output);

///Iterates though all layers in the exact order as the prototxt file describes, calls
///handle_layer method to calculate them and calls cal_post_process method to deal with
///post calculation processes.
bool process_all(std::vector<Layer> const & layer_list, std::vector<std::vector<Tensor *> > const & param_list, std::vector<Tensor *> & data_list, std::string const & output_path,bool is_output);