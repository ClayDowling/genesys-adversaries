#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adversary.h"
#include "parser.h"

// Forward declarations for functions in adversary.c, from the lemon parser
// interface
void *ParseAlloc(void *(*mallocProc)(size_t));
void Parse(void *yyp,             /* The parser */
           int yymajor,           /* The major token code number */
           struct token *yyminor, /* The value for the token */
           struct world_t *       /* Optional %extra_argument parameter */
);
void ParseFree(void *p,                 /* The parser to be deleted */
               void (*freeProc)(void *) /* Function used to reclaim memory */
);

int yylex();

extern char *yytext;
extern int yyleng;
extern FILE *yyin;
extern enum attribute_t yy_adv_attribute;
extern int yy_adv_lineno;

struct world_t *parse_input() {

  struct world_t *world = new_world();

  int yymajor = 0;
  struct token *tok;
  void *parser = ParseAlloc(malloc);
  while (yymajor = yylex()) {

    switch (yymajor) {
    case NUMBER:
      tok = new_token_int(yy_adv_lineno, strtol(yytext, NULL, 10));
      break;
    case ATTRIBUTE:
      tok = new_token_attribute(yy_adv_lineno, yy_adv_attribute);
      break;
    default:
      tok = new_token(yy_adv_lineno, yymajor);
      break;
    }
    tok->strval = strdup(yytext);

    Parse(parser, yymajor, tok, world);
  }
  Parse(parser, 0, NULL, world);
  ParseFree(parser, free);

  return world;
}

struct world_t *parse_file(const char *filename) {
  yyin = fopen(filename, "r");
  return parse_input();
}

struct world_t *parse_buffer(const char *buffer) {
  char template[] = "adversaryXXXXXX";
  yyin = fdopen(mkstemp(template), "w+b");
  fwrite((void*)buffer, strlen(buffer), 1, yyin);
  fflush(yyin);
  rewind(yyin);

  fprintf(stderr, "Writing \"%s\" to %s\n", buffer, template);

  struct world_t *w = parse_input();

  fclose(yyin);
  return w;
}
