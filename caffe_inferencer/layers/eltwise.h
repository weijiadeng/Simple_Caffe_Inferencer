//
// Created by dwj on 8/31/18.
//
///This file offers calculation tools for eltwise layers.

#ifndef CAFFE_INFERENCER_ELTWISE_H
#define CAFFE_INFERENCER_ELTWISE_H

#include "general.h"

///Perform element wise sum operation between two tensors and output the pointer of the resulted tensor
Tensor *eltwise_sum(Tensor *l, Tensor *r);

inline Tensor *eltwise_sum(Tensor *l, Tensor *r) {
  //Map all for two tensors simultaneously
  return tensor_cal_all_bitensor_singleton(l, r, singleton_sum);
}

#endif //CAFFE_INFERENCER_ELTWISE_H
