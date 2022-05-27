#ifndef _LEXER_H_
#define _LEXER_H_

#include <iostream>
#include <string>
using std::string;

extern "C" {
#include "token.h"
}

class lexer {

private:
    std::istream& src;
    unsigned int lineno;
    token* hold;
    bool holding;

    bool string_match(string, string);
    token* get_number(const char);
    string get_identifier(const char);
    token* get_quoted_string(std::istream&);

    enum attribute_t to_attribute(const char*);

public:

    lexer(std::istream&);
    struct token* get();
    void put_back(token&);
    bool eof();
};

#endif
