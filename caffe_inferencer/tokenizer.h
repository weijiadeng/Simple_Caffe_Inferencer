//
// Created by dwj on 9/29/17.
//

///Parse the prototxt file into an ordered list of Tokens.
#ifndef CAFFE_INFERENCER_TOKENIZER_H
#define CAFFE_INFERENCER_TOKENIZER_H

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>


///Trim the string
std::string & trim(std::string & str);



int find_token_end(const std::string & line, int startPos);

///Description of a token
struct Token
{
    enum Type{error,symbol,constant,identifier,_string};

    enum Symbol{not_symbol,leftBrace, rightBrace, leftParenthese, rightParenthese, leftSquareBrace, rightSquareBrace, period,semicolon,colon, equal, _not
    };
    int num;
    std::string text;
    Type gentype;
    Symbol sym;
    Token():num(0),gentype(error),sym(not_symbol){text="";}
};

///The class that parse text into token
class Tokenizer
{
private:
    ///Original code
    std::vector<std::string> code;
    ///Original comment
    std::vector<std::string> comment;
    ///All the std::string const variables.
    std::vector<std::string> strings;

    ///The list of all possible symbols
    std::unordered_map<char, Token::Symbol> symbolList;


    ///Initialize the Tokenizer class. Fill the key word list and
    ///symbol list with pre-defined values.
    void initialize();


    ///The following functions process corresponding kinds of texts.
    void process_line(std::string const &line, uint startPos);
    void process_digit(std::string const &line, uint startPos);
    void process_alpha(std::string const &line, uint startPos);
    void process_punct(std::string const &line, uint startPos);
    void process_string(std::string const &line, uint startPos);



public:
    std::vector<Token> Tokenlist;
    explicit Tokenizer(std::istream & is);

    ///Iterate through all lines and convert each line into tokens.
    void process();

};



#endif //CAFFE_INFERENCER_TOKENIZER_H