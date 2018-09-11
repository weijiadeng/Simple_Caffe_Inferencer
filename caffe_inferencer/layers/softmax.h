//
// Created by dwj on 8/31/18.
//

///Calculate the softmax layer.


#ifndef CAFFE_INFERENCER_SOFTMAX_H
#define CAFFE_INFERENCER_SOFTMAX_H

#include "general.h"

Tensor * softmax(Tensor * tensor, bool is_copy);

inline Tensor * softmax(Tensor * tensor, bool is_copy) {
    Tensor * target = handle_copy(tensor, is_copy);
    //Get the expr sum of all elements
    TYPENAME expr_sum = tensor_reduce_all_singleton(target, singleton_exp_sum, TYPENAME(0));
    tensor_map_all_singleton(target, EXPOWER);
    Multi::set(TYPENAME(1) / expr_sum);
    //Multiply all elements by 1/expr_sum
    tensor_map_all_singleton(target, Multi::call);


    return target;
}

#endif //CAFFE_INFERENCER_SOFTMAX_H
