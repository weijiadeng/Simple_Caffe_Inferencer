#include "prototxt_parser.h"

using boost::algorithm::trim;
using boost::algorithm::split;

using std::unordered_map;
using std::unordered_set;
using std::vector;
using std::istringstream;
using std::istream;
using std::string;



inline Prototxt_item::Prototxt_item(Token const & item) {
    switch (item.gentype) {
        //Is a num, read the num from the token
        case Token::constant:
            val = item.num;
            is_string = false;
            break;
            // Is a string, read the text from the token
        case Token::_string:
        case Token::identifier:
            if (item.text == "true") {
                val = 1;
                is_string = false;
            } else if (item.text == "false") {
                val = 0;
                is_string = false;
            } else {
                str = item.text;
                is_string = true;
            }
            break;
        default:
            //Unexcepted
            printf("Warning!\n");
            break;

        }



}

Prototxt new_prototxt(istream &is, uint &index, Tokenizer const &tokens){
    Prototxt prototxt;
    prototxt.title =  tokens.Tokenlist[index-1].text;
    index++;
    for( ;index<tokens.Tokenlist.size();index++)
    {
        if(tokens.Tokenlist[index].gentype == Token::symbol )
        {
            //"{" indicates new sub prototxx
            if (tokens.Tokenlist[index].sym ==Token::leftBrace) {
                prototxt.sub_item.emplace_back(new_prototxt(is, index,tokens));
            }
            //":" indicates new item

            else if(tokens.Tokenlist[index].sym ==Token::colon){
                if (prototxt.items.count(tokens.Tokenlist[index-1].text) == 0)
                    prototxt.items[tokens.Tokenlist[index-1].text] = vector<Prototxt_item>();
                prototxt.items[tokens.Tokenlist[index-1].text].push_back(Prototxt_item(tokens.Tokenlist[index+1]));
            }
            //":" indicates the end of current prototxt

            else if(tokens.Tokenlist[index].sym ==Token::rightBrace) {

                return prototxt;

            }

        }

    }
    return prototxt;


}

bool read_prototxt(istream &is, vector<Prototxt> &proto_list) {
    if(!is)
        //Can't open file
        return false;
    Tokenizer tokenizer(is);
    tokenizer.process();
    bool is_create = false;
    uint i = 0;
    //Deal with the data input, which is irregular compared to other layers.
    for(;i<tokenizer.Tokenlist.size();i++){
        if(tokenizer.Tokenlist[i].gentype == Token::symbol )
        {
            if (tokenizer.Tokenlist[i].sym ==Token::leftBrace) {
                break;
            }
            else if(tokenizer.Tokenlist[i].sym ==Token::colon){
                if(!is_create) {
                    proto_list.resize(1);
                    is_create = true;
                }
                if (proto_list.back().items.count(tokenizer.Tokenlist[i-1].text) == 0)
                    proto_list.back().items[tokenizer.Tokenlist[i-1].text] = vector<Prototxt_item>();
                proto_list.back().items[tokenizer.Tokenlist[i-1].text].emplace_back(Prototxt_item(tokenizer.Tokenlist[i+1]));


            }

        }
    }
    for(;i<tokenizer.Tokenlist.size();i++) {
        if (tokenizer.Tokenlist[i].gentype == Token::symbol) {
            if (tokenizer.Tokenlist[i].sym == Token::leftBrace) {
                proto_list.emplace_back(new_prototxt(is, i,tokenizer));


            }
        }
    }

    //Modify the "data" or the "input" prototxt to the format of other prototxts

    for(auto & x:proto_list) {
        if (x.items.count("type") == 0) {
            if (x.items.count("input") == 0)
                continue;
            else {
                x.items["name"] =  vector<Prototxt_item>();
                x.items["name"].emplace_back(Prototxt_item(x.items.at("input")[0]));
                x.items["top"] =  vector<Prototxt_item>();
                x.items["top"].emplace_back(Prototxt_item(x.items.at("input")[0]));
                x.items["type"] =  vector<Prototxt_item>();
                x.items["type"].emplace_back();
                x.items["type"].back().is_string = true;
                x.items["type"].back().str = "Input";

                x.items["bottom"] =  vector<Prototxt_item>();
                x.items["bottom"].emplace_back(Prototxt_item(x.items.at("input")[0]));

                continue;

            }
        }

    }
    return true;
}

