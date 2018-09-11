//
// Created by dwj on 9/11/18.
//

///Contains the Image class, which stores the data retrieved from JPEG file.

#ifndef IMAGE_CONVERTER_IMAGE_H
#define IMAGE_CONVERTER_IMAGE_H

#include "interface.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>


class Image {

    //Height of the image
    unsigned long height;
    //Width of the image
    unsigned long width;
    //Image channels
    unsigned short depth;
    //Width*depth
    unsigned long channel_stride;
    //Original image data
    std::vector<int> original_data;
    //Transformed image data
    std::vector<double> output_data;
public:
    //Read image raw data from certain file
    int read_image(const std::string & filename);
    //Separate each channel. In the original data, the same position of different channels are clustered,
    // while in the output format, these data are separated. Thus separation is needed.
    void separate_channel(bool is_revert);
    //Subtract mean value based on given mean value
    int subtract_mean(std::vector<double> const * mean_val);
    //Output file to given filename
    int output_file(const std::string & filename);
};


#endif //IMAGE_CONVERTER_IMAGE_H
