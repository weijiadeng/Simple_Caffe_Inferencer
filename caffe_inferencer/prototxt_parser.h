//
// Created by dwj on 8/31/18.
//

/**
* This file reads the prototxt file and convert it to a json like form.
*/
#pragma once
#include<iostream>
#include<sstream>
#include<string>
#include <utility>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<boost/algorithm/string.hpp>
#include<cstring>
#include"layer.h"
#include "tools.h"
#include "global_val.h"
#include "tokenizer.h"



///An item within a prototxt
struct Prototxt_item {
    std::string str;
    int val;
    //Whether is a string or a value.
    bool is_string;
    explicit Prototxt_item(Token const & item);
    Prototxt_item() = default;

};

struct Prototxt {

    //All items, indexed by their name
    std::unordered_map<std::string,std::vector<Prototxt_item> >  items;
    //All sub items
    std::vector<Prototxt> sub_item;
    std::string title;
    explicit Prototxt(std::string const & str):title(str){}
    Prototxt():items(std::unordered_map<std::string,std::vector<Prototxt_item> >()),sub_item(std::vector<Prototxt> (0)),title(""){}
};


///Create new prototxt instance according to tokens.
Prototxt new_prototxt(std::istream &is, uint &index, Tokenizer const &tokens);

///Convert unstructured tokens into structured prototxts.
bool read_prototxt(std::istream &is, std::vector<Prototxt> &proto_list);

