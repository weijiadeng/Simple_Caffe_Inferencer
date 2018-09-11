
//
// Created by dwj on 8/31/18.
//


#include "layer_decoder.h"
using std::vector;
using std::unordered_map;

void
Layer_decoder::assign_values(unsigned &number, Prototxt const &x, std::string const &item, vector<unsigned int> &col) {
    //No items exist, does not assign value
    if (x.items.count(item) == 0)
    {
        col.resize(0);
        number = 0;
    }
    else {
        //Allocate memory
        col.resize(x.items.at(item).size());
        for (uint i = 0; i < x.items.at(item).size(); i++) {
            //Assign indexes
            col[i] = number_dict.at(x.items.at(item)[i].str);
        }
        number = static_cast<unsigned int>(x.items.at(item).size());
    }
}

void Layer_decoder::assign_params(Layer & ly, Prototxt const & x) {

    switch (ly.type)
    {
        case Layer::Convolution:
            ly.parameters.num_output = x.sub_item[0].items.at("num_output")[0].val;
            ly.parameters.kernel_size = x.sub_item[0].items.at("kernel_size")[0].val;
            ly.parameters.pad = x.sub_item[0].items.at("pad")[0].val;
            ly.parameters.stride = x.sub_item[0].items.at("stride")[0].val;
            if (x.sub_item[0].items.count("bias_term") == 0)
                ly.parameters.bias_term = 2;
            else
                ly.parameters.bias_term = x.sub_item[0].items.at("bias_term")[0].val;
            break;
        case Layer::Pooling:
            ly.parameters.kernel_size = x.sub_item[0].items.at("kernel_size")[0].val;
            ly.parameters.stride = x.sub_item[0].items.at("stride")[0].val;
            ly.parameters.cal_type = cal_type_dict.at(x.sub_item[0].items.at("pool")[0].str);
            break;
        case Layer::InnerProduct:
            ly.parameters.num_output = x.sub_item[0].items.at("num_output")[0].val;
            if (x.sub_item[0].items.count("bias_term") == 0)
                ly.parameters.bias_term = 2;
            else
                ly.parameters.bias_term = x.sub_item[0].items.at("bias_term")[0].val;
            break;
        case Layer::Scale:
            ly.parameters.bias_term = x.sub_item[0].items.at("bias_term")[0].val;
            break;
        case Layer::Data:
        case Layer::Eltwise:
        case Layer::ReLU:
        case Layer::Split:
        case Layer::BatchNorm:
        case Layer::Softmax:
        default:
            break;
    }
}

bool Layer_decoder::decode_prototxt(vector<Prototxt> const &input, vector<Layer> &output) {
    unordered_map<std::string, Layer::Layer_type> type_dict;
    type_dict["Convolution"] = Layer::Convolution;
    type_dict["Data"] = Layer::Data;
    type_dict["Scale"] = Layer::Scale;
    type_dict["Softmax"] = Layer::Softmax;
    type_dict["ReLU"] = Layer::ReLU;
    type_dict["Split"] = Layer::Split;
    type_dict["InnerProduct"] = Layer::InnerProduct;
    type_dict["Eltwise"] = Layer::Eltwise;
    type_dict["BatchNorm"] = Layer::BatchNorm;
    type_dict["Softmax"] = Layer::Softmax;
    type_dict["Pooling"] = Layer::Pooling;
    type_dict["Input"] = Layer::Data;

    cal_type_dict["MAX"] = Layer::Parameter::Max;
    cal_type_dict["AVE"] = Layer::Parameter::Average;

    unsigned i = 0;
    for (auto & x : input)
    {

        if(x.items.count("type")==0)
        //Not a layer
        {
                continue;
        }
        number_dict[x.items.at("name")[0].str] = i++;
    }
    for (auto & x : input) {
        if(x.items.count("type")==0)
        //Not a layer
        {
            continue;
        }
        output.emplace_back();
        output.back().name = x.items.at("name")[0].str;
        output.back().ordinal = number_dict[x.items.at("name")[0].str];
        assign_values(output.back().num_source, x, "bottom", output.back().source);
        assign_values(output.back().num_target, x, "top", output.back().target);


        if(!type_dict.count(x.items.at("type").front().str))
        {
            //Unsupported layer
            printf("Layer %s unsupported!\n",x.items.at("type").front().str.c_str());
            return false;
        }
        output.back().type = type_dict.at(x.items.at("type").front().str);
        output.back().judege_trained();
        assign_params(output.back(), x);
    }
    return true;


}
