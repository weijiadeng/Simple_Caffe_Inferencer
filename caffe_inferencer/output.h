
//
// Created by dwj on 8/31/18.
//


///Deal with data output.


#pragma once

#include<vector>
#include"tensor.h"
#include "global_val.h"




///Write the result of calculated feature map
bool output(Tensor *out, std::string const &layer_name, std::string const &output_path);