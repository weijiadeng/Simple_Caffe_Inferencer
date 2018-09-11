//
// Created by dwj on 8/31/18.
//

///Provide the definition of the Layer class, which is the class

#pragma once
#include <string>
#include "global_val.h"




struct Layer
{
    enum Layer_type {
        Scale, Convolution, ReLU, Pooling, Split, InnerProduct, Eltwise, BatchNorm, Softmax, Data
    } type;
    ///Name of the layer
    std::string name;
    ///Whether the layer has trained weight data
    bool has_trained_val;
    ///The ordinal number of this layer in a network
    unsigned ordinal;
    ///The index array of the source tensors
    std::vector<unsigned>  source;
    ///The total number of sources.
    unsigned num_source;
    ///The index array of the target tensors
    std::vector<unsigned> target;
    ///The total number of targets.
    unsigned num_target;
    struct Parameter{
        ///Whether have bias_terms.
        ///bias_term == 2 means default, different layers have different definition for it.
        ///bias_term == 0 false
        ///bias_term == 1 true
        int bias_term;
        int num_output;
        int kernel_size;
        int pad;
        int stride;
        enum Cal_type{
            Average,Max,Plus
        } cal_type;

    } parameters;


    void judege_trained(){
        switch (type){
            case Scale:
            case Convolution:
            case InnerProduct:
            case BatchNorm:
                has_trained_val = true;
                return;
            default:
                has_trained_val = false;
        }
    }

};