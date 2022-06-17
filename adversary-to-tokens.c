#include "adversary.h"
#include "terminal_tags.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

int yylex(void);

extern FILE *yyin;
extern FILE *yyout;
extern char *yytext;
extern char quoted_string[];

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s testfile.adv\n", argv[0]);
    return EXIT_FAILURE;
  }

  struct lex_context *ctx = lex_file(argv[1]);
  struct token *tok;

  while (tok = lex_scan(ctx)) {
    printf("%s(%s)\n", tag_to_name(tok->token_type), tok->strval);
    destroy_token(tok);
  }
  lex_complete(ctx);

  return EXIT_SUCCESS;
}