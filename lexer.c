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

struct fixed_value attributes[] = {
    {.text = "brawn", .token_type = attr_brawn},
    {.text = "agility", .token_type = attr_agility},
    {.text = "intellect", .token_type = attr_intellect},
    {.text = "cunning", .token_type = attr_cunning},
    {.text = "willpower", .token_type = attr_willpower},
    {.text = "presence", .token_type = attr_presence},
    {.text = "br", .token_type = attr_brawn},
    {.text = "ag", .token_type = attr_agility},
    {.text = "int", .token_type = attr_intellect},
    {.text = "cun", .token_type = attr_cunning},
    {.text = "will", .token_type = attr_willpower},
    {.text = "pr", .token_type = attr_presence}};

struct token *lex_word(struct lex_context *ctx);
int lex_match_keyword(const char *);
struct token *lex_quotedstring(struct lex_context *ctx);

#define ASSEMBLY_SIZE 80
struct lex_context {
  FILE *input;
  char assembly[ASSEMBLY_SIZE];
  int lineno;
};

struct lex_context *lex_file(const char *filename) {
  FILE *in = fopen(filename, "r");
  if (!in) {
    perror(filename);
  }
  return lex_FILE(in);
}

struct lex_context *lex_FILE(FILE* file) {
  struct lex_context *ctx = calloc(1, sizeof(struct lex_context));
  ctx->input = file;
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
    case EOF:
      return 0;
    case ',':
      return new_token(ctx->lineno, COMMA, ",");
    case ':':
      return new_token(ctx->lineno, COLON, ":");
    case '(':
      return new_token(ctx->lineno, LPAREN, "(");
    case ')':
      return new_token(ctx->lineno, RPAREN, ")");
    case '\'':
    case '\"':
      ungetc(c, ctx->input);
      return lex_quotedstring(ctx);
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

  for (int i = 0; i < sizeof(attributes) / sizeof(struct fixed_value); ++i) {
    if (strcasecmp(ctx->assembly, attributes[i].text) == 0)
      return new_token_attribute(ctx->lineno, attributes[i].token_type,
                                 ctx->assembly);
  }

  return new_token(ctx->lineno, WORD, ctx->assembly);
}

struct token* lex_quotedstring(struct lex_context* ctx) {
  char delim = fgetc(ctx->input);
  int startline = ctx->lineno;
  memset(ctx->assembly, 0, ASSEMBLY_SIZE);

  int i=0;
  for(int c = fgetc(ctx->input); i < ASSEMBLY_SIZE; c = fgetc(ctx->input)) {
    switch(c) {
      case EOF:
        fprintf(stderr, "End of File while reading quoted string, line %d", ctx->lineno);
        return NULL;
      case '\n':
        ctx->lineno++;
        ctx->assembly[i++] = c;
        break;
      default:
        if (delim == c) {
          return new_token(startline, QUOTEDSTRING, ctx->assembly);
        }
        ctx->assembly[i++] = c;
        break;

    }
  }
  fprintf(stderr, "Overflow Error: Quoted String longer than %d characters.", ASSEMBLY_SIZE);
  int idx;
  while(idx = fgetc(ctx->input)) {
    if (idx == EOF)  {
      fprintf(stderr, "End of File while reading quoted string, line %d", ctx->lineno);
      return NULL;
    }
    if (idx == '\n') ctx->lineno++;
    if (idx == delim) {
      return new_token(startline, QUOTEDSTRING, ctx->assembly);
    }
  }
}