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
#include "external/half.h"
#include <regex>
#include <algorithm>



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

int judge_version(caffe::NetParameter const & net){
    if(net.layer().size()!=0)
        return 2;
    else if (net.layers().size()!=0)
    {
        if(net.layers(0).has_layer())
            return 0;
        else
            return 1;
    }
}

string name_reconstruct(const string & str){

    string pattern{R"lit([\*?/\\\[\]\{\}\(\)"'`<>;!&])lit"};
    std::regex re(pattern);

    string res{"-"};

    return std::regex_replace(str,re,res);

}

template <typename T>
bool get_parameters_v2(const string & out_path, caffe::NetParameter const & netparam ){
    for (size_t i = 0; i < netparam.layer_size(); i++) {
        if(netparam.layer(i).blobs().empty())
            continue;
        string filename;
        filename = out_path+name_reconstruct(netparam.layer(i).name())+".txt";
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
                ofs<<T(x);
                if(width_count)
                    ofs<<" ";
                else
                    ofs<<endl;
                width_count+=1;
                width_count%=width;
            }
            if(!ofs)
            {
                return false;
            }
        }
    }
    return true;

}

template <typename T>
bool get_parameters_v1(const string & out_path, caffe::NetParameter const & netparam ){
    for (size_t i = 0; i < netparam.layers_size(); i++) {
        int k =netparam.layers().size();
        if(netparam.layers(i).blobs().empty())
            continue;
        string filename;
        filename = out_path+name_reconstruct(netparam.layers(i).name())+".txt";
        printf("Processing %s!\n",filename.c_str());
        std::ofstream ofs(filename.c_str());
        int num = netparam.layers(i).blobs().size();
        ofs<<num<<endl;
        for(int j=0;j<num;j++) {
            ofs<<netparam.layers(i).blobs(j).num()<<" "<<netparam.layers(i).blobs(j).channels()
               <<" "<<netparam.layers(i).blobs(j).height()<<" "<<netparam.layers(i).blobs(j).width()<<endl;
            auto width = netparam.layers(i).blobs(j).width();
            auto width_count= static_cast<int>(1 % width);
            for(auto & x:netparam.layers(i).blobs(j).data()) {
                ofs<<T(x);
                if(width_count)
                    ofs<<" ";
                else
                    ofs<<endl;
                width_count+=1;
                width_count%=width;
            }
            if(!ofs)
            {
                return false;
            }
        }
    }
    return true;
}


template <typename T>
bool get_parameters_v0(const string & out_path, caffe::NetParameter const & netparam ){
   printf("The v0 version caffemodel file is not supported yet!\n");
   return false;

}

template <typename T>
bool get_parameters_form_caffemodel(string const &model_path, string const &out_path) {
    ifstream ifs(model_path, ifstream::in | ifstream::binary);
    if ((&ifs == nullptr) ||(!ifs.is_open())) {
        return false;
    }
    caffe::NetParameter netparam;
    bool ok = netparam.ParseFromIstream(&ifs);

    switch (judge_version(netparam)){
        case 0:
            ok = ok& get_parameters_v1<T>(out_path,netparam);
            break;
        case 1:
            ok = ok& get_parameters_v1<T>(out_path,netparam);
            break;
        case 2:
            ok = ok& get_parameters_v2<T>(out_path,netparam);
            break;
        default:
            ok = false;
            printf("Unsupported caffemodel version!\n");
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

        else if (std::strcmp(arg, "--precision") == 0) // output model folder path
        {
            if (ii + 1 >= argc) {
                // Expecting another parameter
                testAppArgs.show_help = true;
                break;
            }

            testAppArgs.precision = std::string(argv[++ii]);
            //to lower case
            std::transform(testAppArgs.precision.begin(),testAppArgs.precision.end(),testAppArgs.precision.begin(),::tolower);
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

    if(!strcmp(testAppArgs.precision.c_str(), "fp16")){
        testAppArgs.precision_code = FP16;
    }
    else if(!strcmp(testAppArgs.precision.c_str(), "fp32")){
        testAppArgs.precision_code = FP32;
    }
    else if(!strcmp(testAppArgs.precision.c_str(), "fp64")){
        testAppArgs.precision_code = FP64;
    }
    else{
        testAppArgs.show_help = true;
    }



    if (testAppArgs.show_help) {
        printf("Usage: %s [-options] --input <caffemodel_path> --output <output_path>\n", argv[0]);
        printf("where options include:\n");
        printf("    -h                            print this help message\n");
        printf("    --input <caffemodel_path>     caffemodel file path\n");
        printf("    --output <output_file>        output folder path\n");
        printf("    --precision <data_type>       output data precision, currently\n"
               "                                  support fp16, fp32 and fp64\n");
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

    bool ok = true;

    switch (testAppArgs.precision_code){
        case FP16: ok = get_parameters_form_caffemodel<half_float::half>(testAppArgs.input_name, folder_name);
            break;
        case FP32: ok = get_parameters_form_caffemodel<float>(testAppArgs.input_name, folder_name);
            break;
        case FP64: ok = get_parameters_form_caffemodel<double>(testAppArgs.input_name, folder_name);
            break;
    }
    if (!ok) {
        printf("Error when writing files!\n");
        return 0;
    }

    printf("Conversion has been complete!\n");

    return 0;

}

