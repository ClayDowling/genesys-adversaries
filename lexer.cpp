#include "lexer.h"
#include "adversary.h"
#include <cctype>
#include <cstring>
#include <sstream>
using std::istringstream;

lexer::lexer(std::istream& in) : src(in), lineno(1), holding(false), hold(nullptr) {}

struct attributematch {
    string name;
    enum attribute_t attribute;
};

struct token* lexer::get() {

    if (holding) {
        holding = false;
        return hold;
    }

    if (!src) {
        return nullptr;
    }

    if (src.eof()) {
        return nullptr;
    }

    char c = src.get();

    if (EOF == c) {
        return nullptr;
    }

    while(isspace(c)) {
        if ('\n' == c) ++lineno;
        c = src.get();
    }
    if (isdigit(c) || c == '-' || c == '+') {
        return get_number(c);
    }
    if (isalpha(c)) {
        auto n = get_identifier(c);

        if (string_match(n, "skill")) return new_token(lineno, SKILL);
        if (string_match(n, "skills")) return new_token(lineno, SKILL);
        if (string_match(n, "pack")) return new_token(lineno, PACKAGE);
        if (string_match(n, "package")) return new_token(lineno, PACKAGE);
        if (string_match(n, "tal")) return new_token(lineno, TALENT);
        if (string_match(n, "talent")) return new_token(lineno, TALENT);
        if (string_match(n, "talents")) return new_token(lineno, TALENT);
        if (string_match(n, "rival")) return new_token(lineno, RIVAL);
        if (string_match(n, "minion")) return new_token(lineno, MINION);
        if (string_match(n, "nemesis")) return new_token(lineno, NEMESIS);
//        if (string_match(n, "use")) return new_token(lineno, USE);
        
        auto attribute = to_attribute(n.c_str());
        if (attribute != attr_MAX) {
            return new_token_attribute(lineno, attribute);
        }

        // TODO: Throw an exception, this isn't a valid keyword
        return nullptr;
    }
    switch(c) {
        case '(':
            return new_token(lineno, LPAREN);
        case ')':
            return new_token(lineno, RPAREN);
        case ',':
            return new_token(lineno, COMMA);
        case ':':
            return new_token(lineno, COLON);
        case '\"':
            return get_quoted_string(src);
        default:
            // TODO: Throw an exception, unexpected symbol
            return nullptr;
    }
}

string lexer::get_identifier (const char firstletter) {
    std::string name;
    name += firstletter;

    char nc;
    
    while((nc = src.get()) != EOF) {
        if (isalnum(nc)) {
            name += nc;
        } else if (nc == '-') {
            name += nc;
        }
        else {
            src.putback(nc);
            break;
        }
    }

    return name;
}

token* lexer::get_number(const char firstletter) {
    std::string number;
    number += firstletter;

    char nc;
    while((nc = src.get()) != EOF) {
        if (std::isdigit(nc)) {
            number += nc;
        } else {
            src.putback(nc);
            break;
        }
    }

    return new_token_int(lineno, std::stoi(number));
}

token* lexer::get_quoted_string(std::istream& src) {
    string value;

    char c = src.get();
    while(src && c != '\"') {
        value += c;
        c = src.get();
    }
    return new_token_name(lineno, value.c_str());
}

bool lexer::eof() {
    return src.eof();    
}

void lexer::put_back(token& t) {
    holding = true;
    hold = new token(t);
}

enum attribute_t lexer::to_attribute(const char* text) {

    static const attributematch attributes[] = {
            {"brawn", attr_brawn},
            {"br", attr_brawn},
            {"agility", attr_agility},
            {"ag", attr_agility},
            {"intellect", attr_intellect},
            {"int", attr_intellect},
            {"willpower", attr_willpower},
            {"will", attr_willpower},
            {"cunning", attr_cunning},
            {"cun", attr_cunning},
            {"presence", attr_presence},
            {"pr", attr_presence}
    };

    for(auto am : attributes) {
        if (string_match(am.name, text)) return am.attribute;
    }
    return attr_MAX;
}

bool lexer::string_match(string lhs, string rhs) {
    return strcasecmp(lhs.c_str(), rhs.c_str()) == 0;
}

