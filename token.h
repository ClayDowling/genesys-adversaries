#ifndef TOKEN_H
#define TOKEN_H

enum attribute_t {
  attr_brawn,
  attr_agility,
  attr_intellect,
  attr_cunning,
  attr_willpower,
  attr_presence,
  attr_MAX
};

#define END_OF_INPUT -1

struct token {
  int token_type;
  unsigned int lineno;
  char *strval;
  int intval;
  enum attribute_t attributeval;
};

struct token *new_token(unsigned int line, int type, const char *text);
struct token *new_token_word(unsigned int line, const char *text);
struct token *new_token_int(unsigned int line, int val, const char *text);
struct token *new_token_attribute(unsigned int line, enum attribute_t,
                                  const char *text);

void destroy_token(struct token *t);

#endif