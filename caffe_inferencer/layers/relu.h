//
// Created by dwj on 8/31/18.
//
///Calculate the relu layer.

#ifndef CAFFE_INFERENCER_RELU_H
#define CAFFE_INFERENCER_RELU_H

#include "general.h"
///Core relu process
TYPENAME relu_core(TYPENAME t);
///Calculate relu
Tensor * relu(Tensor * tensor, bool is_copy);


inline TYPENAME relu_core(TYPENAME t) {
    if (t > TYPENAME(0))
        return t;
    else
        return TYPENAME(0);
}

inline Tensor * relu(Tensor * tensor, bool is_copy) {
    Tensor * target = handle_copy(tensor, is_copy);

    tensor_map_all_singleton(target, relu_core);
    return target;

}

#endif //CAFFE_INFERENCER_RELU_H
