//
// Created by dwj on 9/11/18.
//

#include "Image.h"


using std::vector;
using std::endl;
using std::string;


int Image::read_image(const string &filename) {
    int ok = get_original_data(filename,original_data,height,width,depth);
    channel_stride = width*height;
    return ok;
}

void Image::separate_channel(bool is_revert) {
    output_data.resize(original_data.size());
    //Convert RGB image to BGR image
    if(is_revert)
        for(int i = 0;i<channel_stride;i++){
            for(int j=0;j<depth;j++)
            {
                output_data[i+channel_stride*j] = original_data[i*depth+depth-1-j];

            }
        }
    else
        for(int i = 0;i<channel_stride;i++){
            for(int j=0;j<depth;j++)
            {
                output_data[i+channel_stride*j] = original_data[i*depth+j];

            }
        }

}

int Image::subtract_mean(vector<double> const * mean_val) {
    if(mean_val== nullptr)
        return 1;
    if (depth != mean_val->size()) {
        printf("Error! The length of the mean value does not equal the channel number of the image!\n");
        return 0;
    }
    for (int i = 0; i < depth; i++)
        for (int j = 0; j < channel_stride; j++)
            output_data[j + i * channel_stride] -= mean_val->at(i);
    return 1;
}

int Image::output_file(const string &filename) {
    std::ofstream ofs(filename);
    if(!ofs){
        printf("Error! Creating output file failed!\n");
        return 0;
    }
    ofs<<"1 "<<depth<<" "<<height<<" "<<width<<endl;

    //Used to mark when it is needed to add a new line. Mod 1 to deal with the case that width == 1
    int width_count = 1%width;


    for(auto x:output_data) {
        //Width != 0, do not need to start a new line
        if(width_count)
            ofs << x << " ";
        //Width ==0, start a new line
        else
            ofs<<x<<endl;
        width_count+=1;
        width_count%=width;
    }
    if(!ofs) {
        printf("Error! Writing file failed!\n");
        return 0;
    }
    return 1;
}
