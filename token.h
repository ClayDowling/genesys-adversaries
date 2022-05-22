#ifndef TOKEN_H
#define TOKEN_H

enum attribute_t {attr_brawn, attr_agility, attr_intellect, attr_cunning, attr_willpower, attr_presence, attr_MAX};

#define END_OF_INPUT -1

struct token {
    int token_type;
    unsigned int lineno;
    union {
        const char* strval;
        int intval;
        enum attribute_t attributeval;
    };
};

struct token* new_token(unsigned int, int);
struct token* new_token_name(unsigned int, const char*);
struct token* new_token_int(unsigned int, int val);
struct token* new_token_attribute(unsigned int, enum attribute_t);

#endif