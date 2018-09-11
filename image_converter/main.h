//
// Created by dwj on 9/11/18.
//

#ifndef IMAGE_CONVERTER_MAIN_H
#define IMAGE_CONVERTER_MAIN_H

#include <string>
#include <vector>

//Default B,G,R mean value. The mean value is gotten from the ilsvrc_2012 data set.
const double DEFAULT_B_MEAN = 104.0069879317889;
const double DEFAULT_G_MEAN = 116.66876761696767;
const double DEFAULT_R_MEAN = 122.6789143406786;


struct TestAppArgs
{
    std::string input_name;
    std::string output_name;
    std::vector<double> mean_val;
    bool show_help;
    TestAppArgs() :
            input_name(""),
            output_name(""),
            mean_val{DEFAULT_B_MEAN,DEFAULT_G_MEAN,DEFAULT_R_MEAN},
            show_help(false)

    {}
};



#endif //IMAGE_CONVERTER_MAIN_H
