#include "model_parser.h"
#include "tools.h"
#include "main.h"

using std::string;
using std::vector;
using std::ifstream;
using std::istream;
using std::istringstream;
using std::cout;

const string MODELNAME_POSTFIX=".txt";

bool get_filename(const string & target, vector<string>& file_list, vector<Layer> const & layer_list)
{
    string inputDirectory = target;

    if(inputDirectory.back()!='/')
        inputDirectory.append("/");
    if(!is_folder_exist(target))
        return false;

    for(auto & x:layer_list)
    {
        if(x.has_trained_val)
            file_list.emplace_back(inputDirectory+x.name+MODELNAME_POSTFIX);
    }

    return true;
}



bool read_model_file(string const & path, vector<vector<Tensor*>>& model_list, Layer_decoder const & layer_decoder) {
    ifstream ifs;
    ifs.open(path);
    if(!ifs)
    {
        printf("Can't open %s!\n",path.c_str());
        return false;
    }

    //Get the filename
    string filename = string(path, path.find_last_of('/') + 1, path.find_last_of('.') - path.find_last_of('/') - 1);
    //Check the ordinal number of the layer.
    int target_num = layer_decoder.number_dict.at(filename);
    string temp;
    getline(ifs, temp);
    //First line of a file records how many tensors are there.
    uint size = string_to_num<uint>(temp);
    getline(ifs, temp);
    while (ifs)
    {
        //Read a single tensor.
        read_tensor(temp, ifs, model_list[target_num]);
        getline(ifs, temp);
        //The size of the tensor is erroneous.
        if (size < model_list[target_num].size()) {
            cout << "Error! Wrong number of parameters at layer number " << target_num << std::endl;
            return false;
        }
    }


    return true;
}

bool read_model(string const & path, vector<vector<Tensor *> > & model_list, Layer_decoder const & ld, vector<Layer> layer_list){

    vector<string> filename_list;
    //Get the file name.
    get_filename(path, filename_list ,layer_list);
    model_list.resize(layer_list.size());
    for (auto & x : filename_list)
        if(!read_model_file(x, model_list, ld))
            return false;
        else
            printf("Read %s done!\n",x.c_str());
    return true;
}

bool read_data_file(string const & path, vector<Tensor*>& data_list) {
    ifstream ifs;
    if(!ifs)
        return false;
    ifs.open(path);
    string temp;
    getline(ifs, temp);
    //Data file only contains one tensor
    return read_tensor(temp, ifs, data_list);
}

bool read_tensor(string & str, istream & ifs, vector<Tensor *> & data_list)
{
    //Trim current sentence
    trim(str);
    if (str.size() == 0)
        return false;
    //Get the shape of current tensor
    vector<int> shape = string_to_array<int>(str);
    //Reverse the order of the shape list
    for (uint i = 0; i < shape.size() / 2; i++) {
        std::swap(shape[i], shape[shape.size() - 1 - i]);
    }
    //Create new tensor
    Tensor * tensor = tensor_init(shape);

    //Array of multiply factor, used to locate the position of the output tensor.
    vector<int> multi(tensor->shape.size());
    vector<int> current_pos(tensor->shape.size());

    for (uint i = 0; i < tensor->shape.size(); i++)
        multi[i] = 1;
    for (uint i = 1; i < tensor->shape.size(); i++) {
        multi[i] = multi[i - 1] * tensor->shape[i];
    }

    //A single input line is called a slice here
    int count_slices = 0;

    while (ifs){
        if (count_slices >= multi[tensor->shape.size() - 1])
            //Already got enough slices.
            break;

        for (uint i = 1; i < tensor->shape.size(); i++) {
            current_pos[i] = count_slices / multi[i - 1];
            current_pos[i] %= tensor->shape[i];
        }
        for(int i=0;i<shape[0];i++)
        {
            current_pos[0] = i;
            TYPENAME temp;
            ifs>>temp;
            *(tensor_at(tensor, current_pos)) = temp;
        }
        count_slices++;
    }
    data_list.push_back(tensor);


    /*
   getline(ifs, str);
   while (ifs)
   {
       if (str.empty())
           continue;
       trim(str);
       if (count_slices >= multi[tensor->shape.size() - 1])
           //Already get enough slices.
           break;
       vector<TYPENAME> data = string_to_array<TYPENAME>(str);
       //Get current position of the output file
       for (int i = 1; i < tensor->shape.size(); i++) {
           current_pos[i] = count_slices / multi[i - 1];
           current_pos[i] %= tensor->shape[i];
       }
       for (int i = 0; i < data.size(); i++)
       {
           current_pos[0] = i;
           *(tensor_at(tensor, current_pos)) = data[i];
       }
       count_slices++;
       getline(ifs, str);
   }*/

    return true;
}
