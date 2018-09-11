//
// Created by dwj on 8/31/18.
//

///This file offers calculation tools for convolution layers.

#ifndef CAFFE_INFERENCER_CONV_H
#define CAFFE_INFERENCER_CONV_H

#include "general.h"

///The function that calculate the convolution within a kernel size
TYPENAME calculate_conv_core(Tensor *input, Tensor * core, std::vector<int> const & offset, int core_num, Tensor * bias);



///The function that calculate the convolution
Tensor * conv(Tensor * tensor_img, Tensor * tensor_core, Tensor * bias, int pad, int stride);

///The function that inference the resulting image shape of a convolution.
void infer_conv_shape(Tensor * tensor_img, std::vector<int>& result_dim, int x_core_dim, int y_core_dim, int z_core_dim, int pad, int stride);


inline TYPENAME calculate_conv_core(Tensor * input, Tensor * core, std::vector<int> const & offset, int core_num, Tensor * bias)
{

    TYPENAME result = TYPENAME(0);
    if (bias != nullptr)
        //Add bias to results.
        result = bias->sub_array[0].sub_array[0].sub_array[0].element[core_num];
    else
        result = TYPENAME(0);
    //Current position for the input feature map
    std::vector<int> input_pos(input->shape.size());
    //Current position for the convolution core
    std::vector<int> core_pos(input->shape.size());
    //Current position for the resulting feature map
    std::vector<int> input_cal_pos(input->shape.size());
    input_pos[3] = 0;
    //Currently calculating core
    core_pos[3] = core_num;

    //For each channel
    for (input_pos[2] = 0, core_pos[2] = 0; core_pos[2] < core->shape[2]; input_pos[2]++, core_pos[2]++)
        //For each height
        for (input_pos[1] = 0, core_pos[1] = 0; core_pos[1] < core->shape[1]; input_pos[1]++, core_pos[1]++)
            //For each width
            for (input_pos[0] = 0, core_pos[0] = 0; core_pos[0] < core->shape[0]; input_pos[0]++, core_pos[0]++)
            {
                //Adjust the position according to the offset.
                add_array(input_pos, offset, input_cal_pos, input->shape.size());
                //Calculation step.
                result += tensor_read(input, input_cal_pos)*(*tensor_at(core, core_pos));
            }

    return result;
}



inline void infer_conv_shape(Tensor * tensor_img, std::vector<int>& result_dim, int x_core_dim, int y_core_dim, int z_core_dim, int pad, int stride)
{
    for (uint i = 0; i < tensor_img->shape.size(); i++)
        result_dim[i] = tensor_img->shape[i];

    result_dim[0] = int_ceil ((tensor_img->shape[0] - x_core_dim + pad + pad) , stride)+1;
    result_dim[1] = int_ceil ((tensor_img->shape[1] - y_core_dim + pad + pad) , stride)+1;
    result_dim[2] = z_core_dim;
    result_dim[3] = tensor_img->shape[3];
}

inline Tensor * conv(Tensor * tensor_img, Tensor * tensor_core, Tensor * bias, int pad, int stride) {

    //Position for the input feature map
    std::vector<int> input_pos(tensor_img->shape.size());
    //Position for the output feature map
    std::vector<int> output_pos(tensor_img->shape.size());
    //The shape of the result
    std::vector<int> result_dim(4);


    infer_conv_shape(tensor_img, result_dim, tensor_core->shape[0], tensor_core->shape[1], tensor_core->shape[3], pad, stride);

    Tensor * result = tensor_init(result_dim);
    int current_core = 0;

    //Each picture
    for (input_pos[3] = 0, output_pos[3] = 0; input_pos[3] < tensor_img->shape[3]; ++input_pos[3], ++output_pos[3])
        //Each core
        for (current_core = 0, output_pos[2] = 0; current_core<tensor_core->shape[3]; ++current_core, ++output_pos[2])
            //Each channel(useless,reserved for possiblie function change)
            for (input_pos[2] = 0; input_pos[2] + tensor_core->shape[2] <= tensor_img->shape[2]; ++input_pos[2])
                //Each Y axis
                for (input_pos[1] = -pad, output_pos[1] = 0; input_pos[1] + tensor_core->shape[1] <= tensor_img->shape[1] + pad; input_pos[1] += stride, ++output_pos[1])
                    //Each X axis
                    for (input_pos[0] = -pad, output_pos[0] = 0; input_pos[0] + tensor_core->shape[0] <= tensor_img->shape[0] + pad; input_pos[0] += stride, ++output_pos[0])
                        *(tensor_at(result, output_pos)) = calculate_conv_core(tensor_img, tensor_core, input_pos, current_core, bias);


    return result;
}

#endif //CAFFE_INFERENCER_CONV_H
