
/**
*Provide basic tools for the facilitation
 of this project
*/

#pragma once
#include<math.h>
#include<vector>
#include<string>
#include<sstream>
#include<iostream>
#include"global_val.h"


///Used to map/reduce assignment to a constant value operation
struct Assign {
    static TYPENAME assign_val;
    static void set(TYPENAME input);
    static TYPENAME call(TYPENAME input);
};

///Used to map/reduce multiple by a constant value operation
struct Multi {
    static TYPENAME multi_base;
public:
    static void set(TYPENAME input);
    static TYPENAME call(TYPENAME input);
};


///custom exp function
TYPENAME exp_custom(TYPENAME x);

///used to count the number of points in a given tensor
TYPENAME singleton_count(TYPENAME l, TYPENAME r);

///Element-wise add between two arrays.
void add_array(const int * left, const int * right, int * result, int array_length);

///Element-wise add between two arrays.
void add_array(std::vector<int> const & left, std::vector<int> const & right, std::vector<int> & result, int array_length);

///Used to map/reduce sum operation
TYPENAME singleton_sum(TYPENAME l, TYPENAME r);

///Used to map/reduce sum epsilon operation
TYPENAME singleton_add_eps(TYPENAME input);

///Used to map/reduce square root operation
TYPENAME singleton_square_root(TYPENAME input);

///Used to map/reduce compare operation
TYPENAME greater(TYPENAME l, TYPENAME r);

///Used to map/reduce sum exp operation
TYPENAME singleton_exp_sum(TYPENAME l, TYPENAME r);

///Divide input by divider and ceil the result
template <typename T>
T int_ceil(T input, T divider) {
    return (input+divider-1)/divider;
}

///Check whether two arrays are exactly the same
bool compare_array(const unsigned * left, unsigned left_size, const unsigned * right, unsigned right_size);

bool compare_array(std::vector<unsigned> const & left, unsigned left_size, std::vector<unsigned> const & right, unsigned right_size);


///Convert num to string, aka itoa
std::string num_to_string(int num);


///Deep copy an array
template<typename T>
void array_deep_copy(T * source, T * target, int start_pos, int end_pos, int offset) {
    for (unsigned i = start_pos; i<end_pos; i++) {
        target[i + offset] = source[i];
    }
}

///Convert string to num, aka atoi
template <typename T>
T string_to_num(std::string const & input) {
    std::istringstream iss(input);
    T output;
    iss >> output;
    return output;
}


//An customize exp function
inline TYPENAME exp_custom(TYPENAME x) {
    x = TYPENAME(1.0) + x / TYPENAME(1024);
    x *= x; x *= x; x *= x; x *= x;
    x *= x; x *= x; x *= x; x *= x;
    x *= x; x *= x;
    return x;

}

inline TYPENAME assign_from_val(TYPENAME val) {
    return val;
}

inline TYPENAME singleton_count(TYPENAME l, TYPENAME r)
{
    return l + 1.0;
}

inline void add_array(const int * left, const int * right, int * result, int array_length) {
    for (int i = 0; i < array_length; ++i)
        result[i] = left[i] + right[i];
}

inline void add_array(std::vector<int> const & left, std::vector<int> const & right, std::vector<int>& result, int array_length) {
    for (int i = 0; i < array_length; ++i)
        result[i] = left[i] + right[i];
}

inline TYPENAME singleton_sum(TYPENAME l, TYPENAME r) {
    return l + r;
}

inline TYPENAME singleton_add_eps(TYPENAME input) {
    return input + CUSTOMEPS;
}

inline TYPENAME singleton_square_root(TYPENAME input) {
    return SQRT(input);
}



inline TYPENAME greater(TYPENAME l, TYPENAME r) {
    return l > r ? l : r;
}



inline TYPENAME singleton_exp_sum(TYPENAME l, TYPENAME r) {
    return l + EXPOWER(r);
}



inline bool compare_array(const unsigned * left, unsigned left_size, const unsigned * right, unsigned right_size) {
    if (right_size != left_size)
        return false;
    for (uint i = 0; i < left_size; i++)
        if (left[i] != right[i])
            return false;
    return true;
}

inline bool compare_array(std::vector<unsigned> const & left, unsigned left_size, std::vector<unsigned> const & right, unsigned right_size) {
    if (right_size != left_size)
        return false;
    for (uint i = 0; i < left_size; i++)
        if (left[i] != right[i])
            return false;
    return true;
}


inline std::string num_to_string(int num) {
    std::ostringstream oss;
    oss << num;
    return oss.str();
}

