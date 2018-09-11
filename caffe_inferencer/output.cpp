
//
// Created by dwj on 8/31/18.
//


#include "output.h"
#include<iostream>
#include<sstream>
#include<string>
#include<fstream>

using std::ofstream;
using std::vector;
using std::ostringstream;
using std::string;
using std::endl;
using std::ostream;



bool output(Tensor *out, string const &layer_name, string const &output_path) {
    if (out == nullptr)
        return true;
    ofstream ofs;
    ofs.open(output_path+ layer_name + ".txt");
    if(!ofs)
        return false;
    //Reverse the shape order
    for (int i = out->shape.size() - 1; i >0; i--)
    {
        ofs << out->shape[i] << " ";
    }
    ofs << out->shape[0] << std::endl;

    for (int i = 0; i < out->shape.back(); i++)
        for (int j = 0; j < out->shape[2]; j++)
            for (int k = 0; k < out->shape[1]; k++)
            {
                for (int l = 0; l < out->shape[0]; l++)
                    ofs << out->sub_array[i].sub_array[j].sub_array[k].element[l] << " ";
                ofs << std::endl;
            }
    ofs.close();
    return true;
}
