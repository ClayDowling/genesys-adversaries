#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

void *parse_input(struct lex_context *ctx) {

  struct world_t *world = new_world();

  int yymajor = 0;
  struct token *tok;
  void *parser = ParseAlloc(malloc);
  while (tok = lex_scan(ctx)) {
    Parse(parser, tok->token_type, tok, world);
  }
  Parse(parser, 0, NULL, world);
  ParseFree(parser, free);

  lex_complete(ctx);
  return world;
}

void * parse_file(const char *filename) {
  struct lex_context *ctx = lex_file(filename);
  return parse_input(ctx);
}

void * parse_buffer(const char *buffer) {
  char filename[16];
  strncpy(filename, "adversaryXXXXXX", sizeof(filename));
  int fd = mkstemp(filename);
  if (fd == -1) {
    perror(filename);
    exit(1);
  }
  if (write(fd, (void*)buffer, strlen(buffer)) == -1) {
    perror("write");
    exit(2);
  }
  close(fd);

  struct lex_context *ctx = lex_file(filename);

  struct world_t *w = parse_input(ctx);

  remove(filename);

  return w;
}
