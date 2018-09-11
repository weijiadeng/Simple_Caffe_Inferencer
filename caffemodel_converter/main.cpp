#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "external/caffe.pb.h"
#include "google/protobuf/text_format.h"
#include <sys/stat.h>
#include "dirent.h"
#include "main.h"
#include <cstring>
#include <sstream>

using std::string;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::endl;
using std::cout;

const int SUPPOSED_DIM = 4;

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

std::string num_to_string(int num) {
    std::ostringstream oss;
    oss << num;
    return oss.str();
}


bool get_parameters_form_caffemodel(string const &model_path, string const &out_path) {
    ifstream ifs(model_path, ifstream::in | ifstream::binary);
    if ((&ifs == nullptr) ||(!ifs.is_open())) {
        return false;
    }
    caffe::NetParameter netparam;
    bool ok = netparam.ParseFromIstream(&ifs);
    for (size_t i = 0; i < netparam.layer_size(); i++) {
        if(netparam.layer(i).blobs().empty())
            continue;
        string filename;
        filename = out_path+netparam.layer(i).name()+".txt";
        printf("Processing %s!\n",filename.c_str());
        std::ofstream ofs(filename.c_str());
        int num = netparam.layer(i).blobs().size();
        ofs<<num<<endl;
        for(int j=0;j<num;j++) {
            auto size = netparam.layer(i).blobs(j).shape().dim().size();
            if(size<SUPPOSED_DIM)
                for(int i=0;i<SUPPOSED_DIM-size;i++){
                ofs<<"1 ";
            }
            for(int k = 0;k<size-1;k++) {
                ofs<<netparam.layer(i).blobs(j).shape().dim(k)<<" ";
            }
            ofs<<netparam.layer(i).blobs(j).shape().dim(size-1)<<endl;

            auto width = netparam.layer(i).blobs(j).shape().dim(size-1);
            auto width_count= static_cast<int>(1 % width);
            for(auto & x:netparam.layer(i).blobs(j).data()) {
                if(width_count)
                    ofs<<x<<" ";
                else
                    ofs<<x<<endl;
                width_count+=1;
                width_count%=width;
            }
            if(!ofs)
            {
                return false;
            }
        }
    }
    ifs.close();
    google::protobuf::ShutdownProtobufLibrary();
    return true;
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
        }
        else if (std::strcmp(arg, "--input") == 0) // input caffemodel file
        {
            if (ii + 1 >= argc) {
                // Expecting another parameter
                testAppArgs.show_help = true;
                break;
            }

            testAppArgs.input_name = std::string(argv[++ii]);
        }
        else if (std::strcmp(arg, "--output") == 0) // output model folder path
        {
            if (ii + 1 >= argc) {
                // Expecting another parameter
                testAppArgs.show_help = true;
                break;
            }

            testAppArgs.output_folder = std::string(argv[++ii]);
        }
        else // unknown
        {
            testAppArgs.show_help = true;
            break;
        }
        ii++;
    }

    if (!(strcmp(testAppArgs.input_name.c_str(), ""))) {
        testAppArgs.show_help = true;
    }

    // For output folder ./ or ../, change folder name to ./output or ../output respectively.
    if (!strcmp(testAppArgs.output_folder.c_str(), "./") && strcmp(testAppArgs.output_folder.c_str(), "../")) {
        testAppArgs.output_folder += "output";
    }


    if (testAppArgs.show_help) {
        printf("Usage: %s [-options] --input <caffemodel_path> --output <output_path>\n", argv[0]);
        printf("where options include:\n");
        printf("    -h                            print this help message\n");
        printf("    --input <caffemodel_path>     caffemodel file path\n");
        printf("    --output <output_file>        output folder path\n");
        return 0;

    }


    string folder_name = testAppArgs.output_folder;
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

    if (!get_parameters_form_caffemodel(testAppArgs.input_name, folder_name)) {
        printf("Error when writing files!\n");
        return 0;
    }

    printf("Conversion has been complete!\n");

    return 0;

}
