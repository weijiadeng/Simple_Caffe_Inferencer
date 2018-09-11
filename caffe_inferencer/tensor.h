
/**
*Definition and basic calculation method of the Tensor structure
*/
#pragma once
#include<stdlib.h>
#include<vector>
#include "tools.h"
#include<stack>
#include "global_val.h"


struct Tensor {
    //Sub tensors
    std::vector<Tensor> sub_array;
    //The data element, only valid if the tensor the base level of a whole tensor
    std::vector<TYPENAME> element;
    //The shape of current tensor
    std::vector<int> shape;
};

///Perform singleton operation on all elements of a tensor, and get a copy of the tensor.
Tensor * tensor_cal_all_singletesnor_singleton(Tensor * tensor_l, TYPENAME(*fp)(TYPENAME l));

///Allocate memory of a tensor
void allocate(Tensor & tensor);

///Initialize a tensor
Tensor * tensor_init(std::vector<int> const & size);

///Copy a tensor
Tensor * copy(Tensor * origin);

///Read certain position of a tensor
TYPENAME tensor_read(Tensor * data, std::vector<int> const & position);

///Read or modify certain position of a tensor
TYPENAME * tensor_at(Tensor * data, std::vector<int> const & position);


///Map singleton operation on a given area of a tensor
void tensor_map_area_singleton_cal(Tensor * tensor,
                                   TYPENAME(*fp)(TYPENAME input),
                                   std::vector<int> const & offset,
                                   std::vector<int> const & area_size);

///Map two-parameter operation on the singletons of two tensors simultaneously.
Tensor * tensor_cal_all_bitensor_singleton(Tensor * tensor_l,
                                           Tensor * tensor_r,
                                           TYPENAME(*fp)(TYPENAME l, TYPENAME r));

///Perform reduce operation on singles of a given area of a tensor
TYPENAME tensor_reduce_area_singleton_cal(Tensor * tensor,
                                          TYPENAME(*fp)(TYPENAME l, TYPENAME r),
                                          TYPENAME init_val,
                                          std::vector<int> const &  offset,
                                          std::vector<int> const & area_size);

///Map area operation on the whole tensor
void tensor_map_area(Tensor * tensor,
                     void(*fp)(Tensor * tensor, std::vector<int> const & offset, std::vector<int> const & area_size),
                     int stride,
                     std::vector<int> const & area_size);

///Map channel-wise operation on the whole tensor
void tensor_map_channel(Tensor * tensor_img,
                        Tensor * var,
                        Tensor * mean,
                        void(*fp)(Tensor * tensor_img, Tensor * var, Tensor * mean, std::vector<int> const & pos));


///Perform reduce operation on singles of a whole tensor
TYPENAME tensor_reduce_all_singleton(Tensor * data,
                                     TYPENAME(*singleton)(TYPENAME, TYPENAME),
                                     TYPENAME init_val);

///Perform map operation on singles of a whole tensor
template<typename T>
void tensor_map_all_singleton(Tensor * data, T(*singleton)(T)) {

    Tensor * current = data;
    //Using stacks to avoid recursion
    static std::stack<Tensor *> st;
    st.push(data);
    while (!st.empty())
    {
        current = st.top();
        st.pop();
        if (current->shape.size() <= 1)
        {
            for (int i = 0;i < current->shape[0]; i++) {
                current->element[i] = (*singleton)(current->element[i]);
            }
        }
        else {
            for (int i = current->shape[current->shape.size() - 1]-1; i >=0 ; i--)
                st.push(&current->sub_array[i]);
        }
    }
}


inline Tensor * tensor_cal_all_singletesnor_singleton(Tensor * tensor_l, TYPENAME(*fp)(TYPENAME l)) {


    Tensor * current_l, *result, *current_result;
    //Using stacks to avoid recursion
    static std::stack<Tensor *> st_l;
    static std::stack<Tensor *> st_result;
    result = tensor_init(tensor_l->shape);
    st_l.push(tensor_l);
    st_result.push(result);

    while (!st_l.empty())
    {
        current_l = st_l.top();
        current_result = st_result.top();
        st_l.pop();
        st_result.pop();
        if (current_l->shape.size() <= 1)
        {
            for (int i = 0; i < current_l->shape[0]; i++) {
                current_result->element[i] = (*fp)(current_l->element[i]);
            }
        }
        else {
            for (int i = current_l->shape.back() -1 ; i >=0; i--) {
                st_l.push(&(current_l->sub_array[i]));
                st_result.push(&(current_result->sub_array[i]));
            }

        }
    }
    return result;
}

inline void allocate(Tensor & tensor) {

    Tensor * current = &tensor;
    //Using stacks to avoid recursion
    static std::stack<Tensor *> st;
    st.push(current);

    while (!st.empty())
    {
        current = st.top();
        st.pop();
        if (current->shape.size() <= 1)
        {
            current->element.resize(current->shape[0]);

        }
        else {
            int sub_count = current->shape.back();
            current->sub_array.resize(current->shape.back());
            for(int i = sub_count-1;i>=0;i--){
                current->sub_array[i].shape.resize(current->shape.size() - 1);
                for (uint j = 0; j < current->shape.size() - 1; j++) {
                    current->sub_array[i].shape[j] = current->shape[j];

                }
                st.push(&current->sub_array[i]);

            }


        }
    }
    //Recursive version
/*    if ((tensor.shape.size()) > 1) {
        int sub_count = tensor.shape.back();
        tensor.sub_array.resize(tensor.shape.back());
        for (int i = 0; i < sub_count; i++) {
            tensor.sub_array[i].shape.resize(tensor.shape.size() - 1);
            for (int j = 0; j < tensor.shape.size() - 1; j++) {
                tensor.sub_array[i].shape[j] = tensor.shape[j];

            }
            allocate(tensor.sub_array[i]);
        }
    }
    else {
        tensor.element.resize(tensor.shape[0]);
    }*/
}

inline Tensor * tensor_init(std::vector<int> const & size) {
    Tensor * data = new Tensor;
    data->shape.resize(size.size());
    for (uint i = 0; i < size.size(); i++)
        data->shape[i] = size[i];
    allocate(*data);
    return data;
}



inline Tensor * copy(Tensor * origin) {
    //Assign from val is a simple function that return the value itself.
    return tensor_cal_all_singletesnor_singleton(origin, assign_from_val);
}

inline TYPENAME tensor_read(Tensor * data, std::vector<int> const & position) {
    for (uint i = 0; i < position.size(); i++)
        if (position[i] < 0 || position[i] >= data->shape[i])
            return 0;
    for (uint i = data->shape.size() - 1; i > 0; i--) {
        data = &(data->sub_array[position[i]]);
    }
    return data->element[position[0]];
}

inline TYPENAME * tensor_at(Tensor * data, std::vector<int> const & position) {
    for (uint i = 0; i < position.size(); i++)
        if (position[i] < 0 || position[i] >= data->shape[i])
            return 0;
    for (uint i = data->shape.size() - 1; i > 0; i--) {
        data = &(data->sub_array[position[i]]);
    }
    return &data->element[position[0]];
}

inline Tensor * tensor_cal_all_bitensor_singleton(Tensor * tensor_l, Tensor * tensor_r, TYPENAME(*fp)(TYPENAME l, TYPENAME r)) {
    Tensor * current_l, *current_r, *result, *current_result;
    static std::stack<Tensor *> st_l;
    static std::stack<Tensor *> st_r;
    static std::stack<Tensor *> st_result;
    result = tensor_init(tensor_l->shape);
    st_l.push(tensor_l);
    st_r.push(tensor_r);
    st_result.push(result);

    while (!st_l.empty())
    {
        current_l = st_l.top();
        current_r = st_r.top();
        current_result = st_result.top();
        st_l.pop();
        st_r.pop();
        st_result.pop();
        if (current_l->shape.size() <= 1)
        {
            for (int i = 0; i < current_l->shape[0]; i++) {
                current_result->element[i] = (*fp)(current_l->element[i], current_r->element[i]);
            }
        }
        else {
            for (int i = 0; i < current_l->shape.back(); i++) {
                st_l.push(&current_l->sub_array[i]);
                st_r.push(&current_r->sub_array[i]);
                st_result.push(&current_result->sub_array[i]);
            }

        }
    }
    return result;
}

inline TYPENAME tensor_reduce_area_singleton_cal(Tensor * tensor,
                                                 TYPENAME(*fp)(TYPENAME l, TYPENAME r), TYPENAME init_val,
                                                 std::vector<int> const & offset,
                                                 std::vector<int> const & area_size) {
    std::vector<int> current_pos(tensor->shape.size());
    std::vector<int> cal_pos(tensor->shape.size());
    TYPENAME result = init_val;
    for (uint i = 0; i < tensor->shape.size(); ++i)
        current_pos[i] = 0;
    for (current_pos[1] = 0; current_pos[1]<area_size[1]; ++current_pos[1])
        for (current_pos[0] = 0; current_pos[0] < area_size[0]; ++current_pos[0])
        {
            add_array(current_pos, offset, cal_pos, tensor->shape.size());
            result = fp(result, tensor_read(tensor, cal_pos));
        }

    return result;
}

inline void tensor_map_area(Tensor * tensor, void(*fp)(Tensor *tensor,
                                                       std::vector<int> const &offset,
                                                       std::vector<int> const &area_size),
                            int stride,
                            std::vector<int> const & area_size) {

    std::vector<int> current_pos(tensor->shape.size());
    //Each picture
    for (current_pos[3] = 0; current_pos[3] + area_size[3] <= tensor->shape[3]; ++current_pos[3])
        //Each channel
        for (current_pos[2] = 0; current_pos[2] + area_size[2] <= tensor->shape[2]; ++current_pos[2])
            //Each Y
            for (current_pos[1] = 0; current_pos[1] + area_size[1] <= tensor->shape[1]; current_pos[1] += stride)
                //Each X
                for (current_pos[0] = 0; current_pos[0] + area_size[0] <= tensor->shape[0]; ++current_pos[0] += stride)
                    (*fp)(tensor, current_pos, area_size);

}

inline void tensor_map_channel(Tensor * tensor_img,
                               Tensor * var,
                               Tensor * mean,
                               void(*fp)(Tensor *tensor_img, Tensor *var, Tensor *mean, std::vector<int> const &pos)) {

    std::vector<int> input_pos(tensor_img->shape.size());

    //Each picture
    for (input_pos[3] = 0; input_pos[3] < tensor_img->shape[3]; ++input_pos[3])
        //Each channel(useless,reserved for possiblie function change)
        for (input_pos[2] = 0; input_pos[2] < tensor_img->shape[2]; ++input_pos[2])
            //Each Y axis
            for (input_pos[1] = 0; input_pos[1] < tensor_img->shape[1]; ++input_pos[1])
                //Each X axis
                for (input_pos[0] = 0; input_pos[0] < tensor_img->shape[0]; ++input_pos[0])
                    (*fp)(tensor_img, var, mean, input_pos);

    return;
}

inline TYPENAME tensor_reduce_all_singleton(Tensor * data,
                                            TYPENAME(*singleton)(TYPENAME, TYPENAME),
                                            TYPENAME init_val) {

    Tensor * current = data;
    static std::stack<Tensor *> st;
    st.push(data);
    while (!st.empty())
    {
        current = st.top();
        st.pop();
        if (current->shape.size() <= 1)
        {
            for (int i = 0;
                 i < current->shape[0]; i++) {
                init_val = (*singleton)(init_val, current->element[i]);
            }
        }
        else {
            for (int i = 0; i < current->shape[current->shape.size() - 1]; i++)
                st.push(&current->sub_array[i]);
        }
    }
    return init_val;
}

inline void tensor_map_area_singleton_cal(Tensor * tensor,
                                          TYPENAME(*fp)(TYPENAME input),
                                          std::vector<int> const & offset,
                                          std::vector<int> const & area_size) {
    std::vector<int> current_pos(tensor->shape.size());
    std::vector<int> cal_pos(tensor->shape.size());
    for (uint i = 0; i < tensor->shape.size(); ++i)
        current_pos[i] = 0;
    for (current_pos[1] = 0; current_pos[1]<area_size[1]; ++current_pos[1])
        for (current_pos[0] = 0; current_pos[0] < area_size[0]; ++current_pos[0])
        {
            add_array(current_pos, offset, cal_pos, tensor->shape.size());
            *(tensor_at(tensor, cal_pos)) = (*fp)(*(tensor_at(tensor, cal_pos)));
        }
}








