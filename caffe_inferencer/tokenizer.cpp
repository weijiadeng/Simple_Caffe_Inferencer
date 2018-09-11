#include "tools.h"
#include"tokenizer.h"
#include <sstream>

using std::vector;
using std::istream;
using std::unordered_map;
using std::string;


void Tokenizer::process() {
    for (auto & line : code)
        process_line(line, 0);
}



Tokenizer::Tokenizer(istream &is) {
    initialize();
    string temp;
    getline(is, temp);
    while (is)
    {
        trim(temp);
        if (!temp.empty())
            code.emplace_back(temp);
        getline(is, temp);
    }
}




///Convert certain line to tokens
void Tokenizer::process_line(string const &line, uint startPos)
{
    if (startPos >= line.size())
        return;
    else if (isdigit(line[startPos]))
        process_digit(line, startPos);
    else if (isalpha(line[startPos]) || line[startPos] == '_')
        process_alpha(line, startPos);
    else if (line[startPos] == '\"')
        process_string(line, startPos);
    else if (ispunct(line[startPos]))
        process_punct(line, startPos);
        //Process the next word
    else if (isblank(line[startPos]))
        process_line(line, ++startPos);
}

///Convert digits to token
void Tokenizer::process_digit(string const &line, uint startPos)
{
    if (startPos >= line.size())
        return;

    //Find the range of current token
    int endPos = find_token_end(line,startPos);

    if (isdigit(line[endPos]))
        endPos++;
    string num;
    for (int i = startPos; i < endPos; i++)
        num.push_back(line[i]);
    Tokenlist.emplace_back(Token());
    Tokenlist.back().gentype = Token::constant;
    Tokenlist.back().num = string_to_num<int>(num);

    //Process the next word
    process_line(line, endPos);
}

inline void Tokenizer::process_alpha(string const &line, uint startPos)
{
    if (startPos >= line.size())
        return;
    //Find the range of current token
    int endPos = find_token_end(line,startPos);

    if ((isalpha(line[endPos])) || (isdigit(line[endPos])) || (line[endPos] == '_'))
        endPos++;
    string word;
    for (int i = startPos; i < endPos; i++)
        word.push_back(line[i]);

    Tokenlist.emplace_back(Token());
    Tokenlist.back().gentype = Token::identifier;
    Tokenlist.back().text = word;

    process_line(line, endPos);
}

///Process punctuations like ";", "," and so on
inline void Tokenizer::process_punct(string const &line, uint startPos)
{
    if (startPos >= line.size())
        return;
    Tokenlist.emplace_back(Token());
    Tokenlist.back().gentype = Token::symbol;
    Tokenlist.back().sym = symbolList[line[startPos]];
    process_line(line, ++startPos);
}


///Process const string value
inline void Tokenizer::process_string(string const &line, uint startPos)
{
    if (startPos >= line.size())
        return;

    uint endPos = startPos + 1;
    for (; endPos < line.size(); endPos++)
        if (line[endPos] == '\"')
            break;
    endPos++;
    string text;
    for (uint i = startPos+1; i < endPos-1; i++)
        text.push_back(line[i]);
    Tokenlist.emplace_back(Token());
    Tokenlist.back().gentype = Token::_string;
    Tokenlist.back().text = text;
    process_line(line, endPos);
}

void Tokenizer::initialize()
{
   using std::make_pair;
    symbolList.emplace(make_pair('{', Token::leftBrace));
    symbolList.emplace(make_pair('}', Token::rightBrace));
    symbolList.emplace(make_pair('(', Token::leftParenthese));
    symbolList.emplace(make_pair(')', Token::rightParenthese));
    symbolList.emplace(make_pair('[', Token::leftSquareBrace));
    symbolList.emplace(make_pair(']', Token::rightSquareBrace));
    symbolList.emplace(make_pair('.', Token::period));
    symbolList.emplace(make_pair(':', Token::colon));
    symbolList.emplace(make_pair(';', Token::semicolon));
    symbolList.emplace(make_pair('=', Token::equal));
    symbolList.emplace(make_pair('~', Token::_not));

}


string &trim(string &str) {
    if (str.empty()) {
        return str;
    }

    string::iterator c;
    // Erase whitespace before the string

    for (c = str.begin(); c != str.end() && isspace(*c++););
    str.erase(str.begin(), --c);

    // Erase whitespace after the string

    for (c = str.end(); c != str.begin() && isspace(*--c););
    str.erase(++c, str.end());

    return str;
}


///Find the range of current token
int find_token_end(const string &line, int startPos) {
    uint endPos = startPos + 1;
    for (; endPos < line.size(); endPos++)
        if (line[endPos] == '\"'||line[endPos]==':'||line[endPos]=='{'||line[endPos]=='}'||line[endPos]==' ')
            break;
    return endPos;
}
