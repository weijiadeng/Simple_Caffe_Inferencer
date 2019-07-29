//
// Created by dwj on 8/31/18.
//
///This file offers calculation tools for bn layers.

#ifndef CAFFE_INFERENCER_BN_H
#define CAFFE_INFERENCER_BN_H

#include "general.h"

///A helper function for bn layers, which deals with bn calculation with a channel.
void bn_helper(Tensor *tensor_img, Tensor *var, Tensor *mean, std::vector<int> const &input_pos);

///The bn calculation function.
Tensor *bn(Tensor *tensor_img, Tensor *variance, Tensor *mean, TYPENAME scale_factor, bool is_copy);

inline void bn_helper(Tensor *tensor_img, Tensor *var, Tensor *mean, std::vector<int> const &input_pos) {

  //Result = (x-mean)/variance
  *(tensor_at(tensor_img, input_pos)) = (*(tensor_at(tensor_img, input_pos))
      - mean->sub_array[0].sub_array[0].sub_array[0].element[input_pos[2]])
      / (var->sub_array[0].sub_array[0].sub_array[0].element[input_pos[2]]);

}

inline Tensor *bn(Tensor *tensor_img, Tensor *variance, Tensor *mean, TYPENAME scale_factor, bool is_copy) {
  Tensor *target = handle_copy(tensor_img, is_copy);
  //Set the multiplication factor
  Multi::set(TYPENAME(scale_factor == 0 ? 1 : 1 / scale_factor));
  //All variances multiply by scale factor
  tensor_map_all_singleton(variance, Multi::call);
  //All means multiply by scale factor
  tensor_map_all_singleton(mean, Multi::call);
  //Perform square root operation on all variances
  tensor_map_all_singleton(variance, singleton_add_eps);
  tensor_map_all_singleton(variance, singleton_square_root);
  //For each channel, perform bn calculation.
  tensor_map_channel(target, variance, mean, bn_helper);

  return target;
}

#endif //CAFFE_INFERENCER_BN_H
