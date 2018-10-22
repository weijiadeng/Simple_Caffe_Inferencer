//
// Created by dwj on 8/31/18.
//

///Calculate the pooling layer.

#ifndef CAFFE_INFERENCER_POOLING_H
#define CAFFE_INFERENCER_POOLING_H

#include "general.h"
#include "conv.h"

#define POOL_HELPER_FUNC_PARAM Tensor * input, Tensor * result, std::vector<int> const & input_pos, std::vector<int> const & out_pos, int kernel_size

///Core function for pooling calculation
TYPENAME calculate_pool_core(Tensor * tensor_img, int kernel_size, std::vector<int> const & pos, TYPENAME init_val, TYPENAME(*fp)(TYPENAME, TYPENAME));

///Helper function for average pool
void avg_pool_helper(POOL_HELPER_FUNC_PARAM);

///Helper function for max pool
void max_pool_helper(POOL_HELPER_FUNC_PARAM);

///Calculate the pooling layer.
Tensor * pool(Tensor * tensor_img, int kernel_size, int stride, void(*fp)(POOL_HELPER_FUNC_PARAM));

inline void infer_pool_shape(Tensor * tensor_img, std::vector<int>& result_dim, int x_core_dim, int y_core_dim, int z_core_dim, int pad, int stride)
{
    for (uint i = 0; i < tensor_img->shape.size(); i++)
        result_dim[i] = tensor_img->shape[i];
    result_dim[0] = int_ceil(tensor_img->shape[0] + 2 * pad - (x_core_dim + 1) + 1,stride) + 1;
    result_dim[1] = int_ceil(tensor_img->shape[1] + 2 * pad - (y_core_dim + 1) + 1,stride) + 1;
    result_dim[2] = z_core_dim;
    result_dim[3] = tensor_img->shape[3];
}


inline TYPENAME calculate_pool_core(Tensor * tensor_img, int kernel_size, std::vector<int> const & pos, TYPENAME init_val, TYPENAME(*fp)(TYPENAME, TYPENAME))
{
    //Init val depends on the kind of the pooling function.
    TYPENAME result = TYPENAME(init_val);
    //The position for the input tensor
    std::vector<int> input_pos(tensor_img->shape.size());
    //The adjusted position for the input tensor
    std::vector<int> input_cal_pos(tensor_img->shape.size());
    input_pos[3] = 0;
    input_pos[2] = 0;
    for (input_pos[1] = 0; input_pos[1] < kernel_size; input_pos[1]++)
        for (input_pos[0] = 0; input_pos[0] < kernel_size; input_pos[0]++)
        {
            //Get adjusted position
            add_array(input_pos, pos, input_cal_pos, tensor_img->shape.size());

            //Calculate the result
            result = fp(result, tensor_read(tensor_img, input_cal_pos));
        }

    return result;
}


inline void avg_pool_helper(POOL_HELPER_FUNC_PARAM)
{
    //The sum of current core
    *tensor_at(result, out_pos) = calculate_pool_core(input, kernel_size, input_pos, TYPENAME(0),singleton_sum);
    //Get the average value
    *tensor_at(result, out_pos) /= (kernel_size*kernel_size);
}

inline void max_pool_helper(POOL_HELPER_FUNC_PARAM)
{
    *tensor_at(result, out_pos) = calculate_pool_core(input, kernel_size, input_pos, TYPENAMEMIN, greater);
}




inline Tensor * pool(Tensor * tensor_img, int kernel_size, int stride, void(*fp)(POOL_HELPER_FUNC_PARAM)) {

    std::vector<int> input_pos(tensor_img->shape.size());
    std::vector<int> output_pos(tensor_img->shape.size());
    std::vector<int> result_dim(4);

    //Get the shape of the result feature map.
    infer_pool_shape(tensor_img, result_dim, kernel_size, kernel_size, tensor_img->shape[2], 0, stride);

    Tensor * result = tensor_init(result_dim);

    int minus_num_x = 0;
    int minus_num_y = 0;
    if((tensor_img->shape[0]-kernel_size)%stride)
        minus_num_x=1;
    if((tensor_img->shape[1]-kernel_size)%stride)
        minus_num_y=1;
    //Each picture
    for (input_pos[3] = 0, output_pos[3] = 0; input_pos[3] < tensor_img->shape[3]; ++input_pos[3], ++output_pos[3])
        //Each channel
        for (input_pos[2] = 0, output_pos[2] = 0; input_pos[2] < tensor_img->shape[2]; ++input_pos[2], ++output_pos[2])
            //Each Y axis
            for (input_pos[1] = 0, output_pos[1] = 0; input_pos[1] + kernel_size -minus_num_y <= tensor_img->shape[1]; input_pos[1] += stride, ++output_pos[1])
                //Each X axis
                for (input_pos[0] = 0, output_pos[0] = 0; input_pos[0] + kernel_size -minus_num_x <= tensor_img->shape[0]; input_pos[0] += stride, ++output_pos[0])
                {
                    (*fp)(tensor_img, result, input_pos, output_pos, kernel_size);

                }

    return result;
}



#endif //CAFFE_INFERENCER_POOLING_H
