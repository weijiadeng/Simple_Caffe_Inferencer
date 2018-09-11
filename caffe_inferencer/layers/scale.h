//
// Created by dwj on 8/31/18.
//

///Calculate the scale layer.


#ifndef CAFFE_INFERENCER_SCALE_H
#define CAFFE_INFERENCER_SCALE_H

#include "general.h"

///Helper function for the scale calculation
void scale_helper(Tensor * tensor_img, Tensor * alpha, Tensor * beta, std::vector<int> const & input_pos);

///Function for the scale calculation

Tensor * scale(Tensor * tensor_img, Tensor * alpha, Tensor * beta, bool is_copy);


inline void scale_helper(Tensor * tensor_img, Tensor * alpha, Tensor * beta, std::vector<int> const & input_pos) {
    //result = input*alpha + beta
    *(tensor_at(tensor_img, input_pos)) = (*(tensor_at(tensor_img, input_pos))
                                           * alpha->sub_array[0].sub_array[0].sub_array[0].element[input_pos[2]])
                                          + (beta->sub_array[0].sub_array[0].sub_array[0].element[input_pos[2]]);
}

inline Tensor * scale(Tensor * tensor_img, Tensor * alpha, Tensor * beta, bool is_copy) {
    Tensor * target = handle_copy(tensor_img, is_copy);

    //Perform Channel-wise operation
    tensor_map_channel(target, alpha, beta, scale_helper);
    return target;
}


#endif //CAFFE_INFERENCER_SCALE_H
