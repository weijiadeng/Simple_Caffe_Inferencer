//
// Created by dwj on 8/31/18.
//
///This file offers general tools for all kinds of layers.

#ifndef CAFFE_INFERENCER_GENERAL_H
#define CAFFE_INFERENCER_GENERAL_H

#include <cstdlib>
#include "../tools.h"
#include "../tensor.h"
#include "../global_val.h"


///Cope the tensor and return the pointer of the copy.
/// If is_copy is false, return the pointer of the origin tensor
Tensor * handle_copy(Tensor * tensor, bool is_copy);

inline Tensor * handle_copy(Tensor * tensor, bool is_copy) {
    Tensor * target;
    if (is_copy)
        target = copy(tensor);
    else
        target = tensor;
    return target;
}


#endif //CAFFE_INFERENCER_GENERAL_H
