#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adversary.h"
#include "lexer.h"
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

struct world_t *parse_input(struct lex_context *ctx) {

  struct world_t *world = new_world();

  int yymajor = 0;
  struct token *tok;
  void *parser = ParseAlloc(malloc);
  while (tok = lex_scan(ctx)) {
    Parse(parser, tok->token_type, tok, world);
  }
  Parse(parser, 0, NULL, world);
  ParseFree(parser, free);

  return world;
}

struct world_t *parse_file(const char *filename) {
  struct lex_context *ctx = lex_file(filename);
  return parse_input(ctx);
  lex_complete(ctx);
}

struct world_t *parse_buffer(const char *buffer) {
  char template[] = "adversaryXXXXXX";
  FILE *src = fdopen(mkstemp(template), "w+b");
  fwrite((void *)buffer, strlen(buffer), 1, src);
  fflush(src);
  rewind(src);

  struct lex_context *ctx = lex_file(template);

  struct world_t *w = parse_input(ctx);

  fclose(src);
  return w;
}
