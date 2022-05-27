#include "token.h"
#include "adversary.h"

#include <stdlib.h>
#include <string.h>

struct token _token_cr = {
    .token_type = 0
};

struct token* TOKEN_CR = &_token_cr;

struct token* new_token(unsigned int l, int t) {
    struct token* tok = (struct token*)calloc(1, sizeof(struct token));
    tok->token_type = t;
    tok->lineno = l;
    return tok;
}

struct token* new_token_name(unsigned int l, const char* n) {
    struct token *tok = new_token(l, NAME);
    tok->strval = strdup(n);
    return tok;
}

struct token* new_token_int(unsigned int l, int i) {
    struct token *tok = new_token(l, NUMBER);
    tok->intval = i;
    return tok;
}

struct token* new_token_attribute(unsigned int l, enum attribute_t a) {
    struct token *tok = new_token(l, ATTRIBUTE);
    tok->attributeval = a;
    return tok;
}


