
//
// Created by dwj on 8/31/18.
//

#include "Image.h"
#include "main.h"
#include <vector>
#include <cstring>
using std::vector;

int main(int argc,char *argv[]) {







    TestAppArgs testAppArgs = TestAppArgs();
    int ii = 1;
    while (true) {
        if (ii >= argc)
            break;
        const char *arg = argv[ii];

        if (std::strcmp(arg, "-h") == 0) // help
        {
            // Print usage
            testAppArgs.show_help = true;
            break;
        }
        else if (std::strcmp(arg, "--input") == 0) // input image file
        {
            if (ii + 1 >= argc) {
                // Expecting another parameter
                testAppArgs.show_help = true;
                break;
            }

            testAppArgs.input_name = std::string(argv[++ii]);
        } else if (std::strcmp(arg, "--output") == 0) // converted image file
        {
            if (ii + 1 >= argc) {
                // Expecting another parameter
                testAppArgs.show_help = true;
                break;
            }

            testAppArgs.output_name = std::string(argv[++ii]);
        }
        else if (std::strcmp(arg, "--mean") == 0) // Mean
        {
            if (ii+1 >= argc)
            {
                // Expecting another parameter
                testAppArgs.show_help = true;
                break;
            }
            char *token;
            int i = 0;
            token = strtok(argv[++ii], ",\n");
            while( token != NULL ) {
                if (i > 3) {
                    printf("Number of mean values should not be greater than 4 \n");
                    testAppArgs.show_help = true;
                    break;
                }
                testAppArgs.mean_val[i] = atof(token);
                token = strtok(NULL, ",\n");
                i++;
            }
        }
        else // unknown
        {
            testAppArgs.show_help = true;
            break;
        }
        ii++;
    }

    if (!(strcmp(testAppArgs.input_name.c_str(), "")  &&
          strcmp(testAppArgs.output_name.c_str(), ""))) {
        testAppArgs.show_help = true;
    }


    if (testAppArgs.show_help) {
        printf("Usage: %s [-options] --input <imgae_file> --output <converted_file> --mean [mean_values]\n", argv[0]);
        printf("where options include:\n");
        printf("    -h                            print this help message\n");
        printf("    --input <imgae_file>          input image file path\n");
        printf("    --output <converted_file>     output txt file path\n");
        printf("    --mean [mean_values]          the mean values needed to be substract\n"
               "                                  if the values are not given, use default values instead.\n ");
        return 0;

    }

    Image image;
    if(!image.read_image(testAppArgs.input_name)) {
        printf("Error occured when loading image file!\n");
        return 0;
    }
    image.separate_channel(true);
    if(!image.subtract_mean(&testAppArgs.mean_val)){
        printf("Error occured when subtracting mean values!\n");
        return 0;
    }
    if(!image.output_file(testAppArgs.output_name)){
        printf("Error occured when outputing file!\n");
        return 0;
    }
    printf("Convert image file finished with success!\n");
    return 0;

}
