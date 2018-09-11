
//
// Created by dwj on 8/31/18.
//


#include<stdlib.h>
#include<queue>
#include<vector>
#include"layer.h"
#include"layer_cal.h"
#include "tensor.h"
#include "output.h"
#include "driver.h"
#include<ctime>

using std::vector;
using std::string;

bool cal_post_process(Layer const & ly, Tensor * result, vector<Tensor*>& data_list, int layer_index,string const& output_path, bool is_output) {

    //Assign the pointer of the result to the first target.
    data_list[ly.target[0]] = result;

    if(is_output)
        //Intermediate result should be dump.
        if(!output(data_list[ly.target[0]], ly.name, output_path))
        //Can't open file.
        {
            printf("Error when writing files!\n");
            return false;

        }
    for (uint i = 1; i < ly.num_target; ++i)
    {
        //Copy the result, then assign the pointer of the copy to other targets.
        Tensor * replicate = copy(result);
        data_list[ly.target[i]] = replicate;
    }
    return true;
}


bool handle_layer(Layer const & ly, int layer_index, vector<vector<Tensor *> > const & param_list, vector<Tensor *> & data_list, string const & output_path, bool is_output){
    Tensor * result = nullptr;

    //Different types of operation have different kinds of behavior
    switch (ly.type)
    {
        case Layer::BatchNorm:
            result = bn(data_list[ly.source[0]], //Feature map
                        param_list[layer_index][1], //Variance
                        param_list[layer_index][0], //Mean
                        param_list[layer_index][2]->sub_array[0].sub_array[0].sub_array[0].element[0], //Scale factor
                        //Check whether the input and the output are the same, if they are not the same,
                        //the original data should not be changed.
                        !compare_array(ly.source, ly.num_source, ly.target, ly.num_target));
            break;

        case Layer::Convolution:
            if (ly.parameters.bias_term)
            //Has bias
            {
                result = conv(data_list[ly.source[0]], //Feature map
                              param_list[layer_index][0], //Weight data
                              param_list[layer_index][1], //Bias data
                              ly.parameters.pad,
                              ly.parameters.stride);
            }
            else
            {
                result = conv(data_list[ly.source[0]],
                              param_list[layer_index][0],
                              nullptr,
                              ly.parameters.pad,
                              ly.parameters.stride);
            }
            break;


        case Layer::Eltwise:
            result = data_list[ly.source[0]];
            for (uint i = 1; i<ly.num_source; ++i)
            {
                //Currently support Eltwise-sum only.
                result = eltwise_sum(result, data_list[ly.source[i]]);
            }
            break;

        case Layer::InnerProduct:
            if (ly.parameters.bias_term)
            {
                result = fc(data_list[ly.source[0]], param_list[layer_index][0], param_list[layer_index][1]);
            }
            else
            {
                result = fc(data_list[ly.source[0]], param_list[layer_index][0], nullptr);
            }
            break;
        case Layer::Pooling:
            if (ly.parameters.cal_type == Layer::Parameter::Average)
                result = pool(data_list[ly.source[0]], ly.parameters.kernel_size, ly.parameters.stride,avg_pool_helper);
            else if (ly.parameters.cal_type == Layer::Parameter::Max)
                result = pool(data_list[ly.source[0]], ly.parameters.kernel_size, ly.parameters.stride, max_pool_helper);
            break;
        case Layer::Softmax:
            result = softmax(data_list[ly.source[0]],
                    //Check whether the input and the output are the same, if they are not the same,
                    //the original data should not be changed.
                             !compare_array(ly.source, ly.num_source, ly.target, ly.num_target));
            break;
        case Layer::Scale:
            result = scale(data_list[ly.source[0]],
                           param_list[layer_index][0],
                           param_list[layer_index][1],
                    //Check whether the input and the output are the same, if they are not the same,
                    //the original data should not be changed.
                           !compare_array(ly.source, ly.num_source, ly.target, ly.num_target));
            break;
        case Layer::ReLU:
            result = relu(data_list[ly.source[0]],
                    //Check whether the input and the output are the same, if they are not the same,
                    //the original data should not be changed.
                          !compare_array(ly.source, ly.num_source, ly.target, ly.num_target));
            break;
        case Layer::Data:
            result = data_list[ly.target[0]];
            break;
        default:
            result = nullptr;
            break;
    }
    //Perform post calculation processes.
    return cal_post_process(ly, result, data_list, layer_index,output_path, is_output);

}

bool process_all(vector<Layer> const & layer_list,
                 vector<vector<Tensor *> > const & param_list,
                 vector<Tensor *> & data_list,
                 string const & output_path,
                 bool is_output)
{
    int start = clock();
    data_list.resize(layer_list.size());
    for (uint i = 0; i<layer_list.size()-1; i++)
    {
        if(!handle_layer(layer_list[i], i, param_list, data_list, output_path, is_output))
            return false;
        printf("Layer %s calculation finished!\n",layer_list[i].name.c_str());

    }
    //Always output the finial result.
    if(!handle_layer(layer_list.back(), layer_list.size()-1, param_list, data_list,output_path, true))
        return false;
    printf("Layer %s finished!\n",layer_list.back().name.c_str());

    int end = clock();
    printf("Computation step takes %f seconds!\n",(float(end)-start) /CLOCKS_PER_SEC);
    return true;

}
