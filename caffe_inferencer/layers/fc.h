//
// Created by dwj on 8/31/18.
//
///This file offers calculation tools for inner product layers.

#ifndef CAFFE_INFERENCER_FC_H
#define CAFFE_INFERENCER_FC_H
#include <omp.h>
#include "general.h"

///The function that calculate the inner product layer.
Tensor *fc(Tensor *tensor_img, Tensor *tensor_core, Tensor *bias);

inline Tensor *fc(Tensor *tensor_img, Tensor *tensor_core, Tensor *bias) {
  //Position for the input feature map
  std::vector<int> input_pos(tensor_img->shape.size());
  //Position for the output feature map
  std::vector<int> output_pos(tensor_img->shape.size());
  //The shape of the result image.
  std::vector<int> result_dim(4);

  for (uint i = 0; i < tensor_img->shape.size(); i++)
    result_dim[i] = tensor_img->shape[i];

  result_dim[0] = tensor_core->shape[1];
  result_dim[1] = tensor_core->shape[3];
  result_dim[2] = 1;
  result_dim[3] = 1;

  Tensor *result = tensor_init(result_dim);
#pragma omp parallel for
  for (int i = 0; i < tensor_core->shape[1]; i++) {

    if (bias == nullptr)
      result->sub_array[0].sub_array[0].sub_array[0].element[i] = 0;
    else
      result->sub_array[0].sub_array[0].sub_array[0].element[i] =
          bias->sub_array[0].sub_array[0].sub_array[0].element[i];

    for (int j = 0; j < tensor_core->shape[0]; j++) {
      result->sub_array[0].sub_array[0].sub_array[0].element[i]
          += tensor_img->sub_array[0].sub_array[j].sub_array[0].element[0]
          * tensor_core->sub_array[0].sub_array[0].sub_array[i].element[j];
    }

  }
  return result;
}

#endif //CAFFE_INFERENCER_FC_H
