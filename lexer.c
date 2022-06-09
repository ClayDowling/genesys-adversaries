#include "lexer.h"
#include "adversary.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct fixed_value {
  const char *text;
  int token_type;
};

struct fixed_value keywords[] = {{.text = "skill", .token_type = SKILL},
                                 {.text = "talent", .token_type = TALENT},
                                 {.text = "package", .token_type = PACKAGE},
                                 {.text = "minion", .token_type = MINION},
                                 {.text = "rival", .token_type = RIVAL},
                                 {.text = "nemesis", .token_type = NEMESIS}};

struct token *lex_word(struct lex_context *ctx);
int lex_match_keyword(const char *);

#define ASSEMBLY_SIZE 80
struct lex_context {
  FILE *input;
  char assembly[ASSEMBLY_SIZE];
  int lineno;
};

struct lex_context *lex_file(const char *filename) {
  struct lex_context *ctx = calloc(1, sizeof(struct lex_context));
  ctx->input = fopen(filename, "r");
  ctx->lineno = 1;
  return ctx;
}

void lex_complete(struct lex_context *ctx) {
  fclose(ctx->input);
  free(ctx);
}

struct token *lex_scan(struct lex_context *ctx) {
  int c;

  while (!feof(ctx->input)) {
    c = fgetc(ctx->input);
    switch (c) {
    case ',':
      return new_token(ctx->lineno, COMMA, ",");
    case ':':
      return new_token(ctx->lineno, COLON, ":");
    case '(':
      return new_token(ctx->lineno, LPAREN, "(");
    case ')':
      return new_token(ctx->lineno, RPAREN, ")");
    case ' ':
    case '\t':
      continue;
    default:

      if (isalpha(c)) {
        ungetc(c, ctx->input);
        return lex_word(ctx);
      }

      fprintf(stderr, "Unknown value in input: %c\n", c);
      break;
    }
  }

  /* At end of input, no tokens */
  return NULL;
}

struct token *lex_word(struct lex_context *ctx) {
  memset(ctx->assembly, 0, ASSEMBLY_SIZE);
  int c;
  for (int i = 0; i < ASSEMBLY_SIZE; ++i) {
    c = fgetc(ctx->input);
    if (isalpha(c)) {
      ctx->assembly[i] = c;
    } else {
      ungetc(c, ctx->input);
      break;
    }
  }

  for (int i = 0; i < sizeof(keywords) / sizeof(struct fixed_value); ++i) {
    if (strcasecmp(ctx->assembly, keywords[i].text) == 0)
      return new_token(ctx->lineno, keywords[i].token_type, ctx->assembly);
  }
  return NULL;
}