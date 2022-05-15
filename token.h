#ifndef TOKEN_H
#define TOKEN_H

enum attribute_t {attr_brawn, attr_agility, attr_intellect, attr_cunning, attr_presence, attr_MAX};

struct token {
    int token_type;
    const char* strval;
    int intval;
    enum attribute_t attributeval;
};

#endif