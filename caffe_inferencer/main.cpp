
//
// Created by dwj on 8/31/18.
//

#include<fstream>
#include "prototxt_parser.h"
#include "driver.h"
#include "layer_decoder.h"
#include "model_parser.h"
#include "output.h"
#include "main.h"
#include <sys/stat.h>
#include "dirent.h"

using std::string;
using std::vector;

//Test whether there exists such folder
int is_folder_exist(const string &path)
{
    DIR *dp;
    if ((dp = opendir(path.c_str())) == NULL)
    {
        return 0;
    }

    closedir(dp);
    return -1;
}



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
        } else if (std::strcmp(arg, "-m") == 0) // drop intermediate files.
        {
            testAppArgs.drop_middle = true;
        } else if (std::strcmp(arg, "--prototxt") == 0) // input prototxt file
        {
            if (ii + 1 >= argc) {
                // Expecting another parameter
                testAppArgs.show_help = true;
                break;
            }

            testAppArgs.proto_name = std::string(argv[++ii]);
        } else if (std::strcmp(arg, "--model_dir") == 0) // input model file folder path
        {
            if (ii + 1 >= argc) {
                // Expecting another parameter
                testAppArgs.show_help = true;
                break;
            }

            testAppArgs.model_path = std::string(argv[++ii]);
        } else if (std::strcmp(arg, "--image") == 0) // input image file path
        {
            if (ii + 1 >= argc) {
                // Expecting another parameter
                testAppArgs.show_help = true;
                break;
            }

            testAppArgs.input_image = std::string(argv[++ii]);
        }
        else if (std::strcmp(arg, "--output") == 0) //  output folder path
        {
            if (ii + 1 >= argc) {
                // Expecting another parameter
                testAppArgs.show_help = true;
                break;
            }

            testAppArgs.output_path = std::string(argv[++ii]);
        } else // unknown
        {
            testAppArgs.show_help = true;
            break;
        }
        ii++;
    }

    if (!(strcmp(testAppArgs.proto_name.c_str(), "")  &&
          strcmp(testAppArgs.input_image.c_str(), ""))) {
        testAppArgs.show_help = true;
    }

    // For output folder ./ or ../, change folder name to ./output or ../output respectively.
    if (!strcmp(testAppArgs.output_path.c_str(), "./") && strcmp(testAppArgs.output_path.c_str(), "../")) {
        testAppArgs.output_path += "output";
    }


    if (testAppArgs.show_help) {
        printf("Usage: %s [-options] --prototxt <prototxt_file> --model_dir <model_path> --image <image_file> --output <output_path>\n", argv[0]);
        printf("where options include:\n");
        printf("    -h                            print this help message\n");
        printf("    -m                            output intermediate results\n");
        printf("    --prototxt <prototxt_file>    prototxt file path\n");
        printf("    --model_dir <model_path>      model files folder path\n");
        printf("    --image <image_file>          image file path\n");
        printf("    --output <output_file>        output folder path\n");
        return 0;

    }


    printf("Start reading layer definitions!\n");

    std::ifstream ifs;
    ifs.open(testAppArgs.proto_name.c_str());
    vector<Prototxt> plist;
    if (!read_prototxt(ifs, plist)) {
        printf("Error when loading the prototxt file!\n");
        return 0;
    }
    ifs.close();

    vector<Layer> layer_list;
    Layer_decoder ld;

    if (!ld.decode_prototxt(plist, layer_list)) {
        printf("Error when parsing the prototxt file!\n");
        return 0;
    }

    printf("Read layer definition done!\n");

    printf("Start reading image file!\n");

    vector<vector<Tensor *> > model_list;
    vector<Tensor *> data_list;
    if (!read_data_file(testAppArgs.input_image, data_list)) {
        printf("Error when loading the image file!\n");
        return 0;

    }
    printf("Read image file done!\n");

    printf("Start reading model files!\n");

    if (!is_folder_exist(testAppArgs.model_path)) {
        printf("Error when loading the model files!\n");
        return 0;

    }
    if(testAppArgs.model_path.back()!='/')
        testAppArgs.model_path.push_back('/');

    if (!read_model(testAppArgs.model_path, model_list, ld, layer_list)) {
        printf("Error when loading the model files!\n");
        return 0;

    }
    printf("Read model files done!\n\n\n");

    printf("Start inferencing!\n");


    string folder_name = testAppArgs.output_path;
    if(folder_name.back() == '/')
        folder_name.resize(folder_name.size()-1);
    string ori_folder_name = folder_name;
    int index = 1;
    while ( is_folder_exist(folder_name)){
        folder_name = ori_folder_name+num_to_string(index);
        printf("Output folder occupied. Output folder changed to %s\n",folder_name.c_str());
        index++;
    }


    if(mkdir(folder_name.c_str(),0755)) {
        printf("Error when creating folder!\n");
        return 0;
    }

    folder_name += "/";

    if (!process_all(layer_list, model_list, data_list, folder_name ,testAppArgs.drop_middle)) {
        printf("Error when inferencing!\n");
        return 0;

    }

    printf("Inference has been complete!\n");

    return 0;

}

