#include "lexer.h"
#include "adversary.h"

#include <limits.h>

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
                                 {.text = "nemesis", .token_type = NEMESIS},
                                 {.text = "weapon", .token_type = WEAPON},
                                 {.text = "damage", .token_type = DAMAGE},
                                 {.text = "dmg", .token_type = DAMAGE},
                                 {.text = "critical", .token_type = CRITICAL},
                                 {.text = "crit", .token_type = CRITICAL}};

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
struct token *lex_end_of_file(struct lex_context *ctx);
struct token *lex_use_file(struct lex_context *ctx, char* filename);
void lex_error(struct lex_context* ctx, const char* message);
struct token *lex_number(struct lex_context *ctx);

struct lex_pushed_context {
  FILE *input;
  int lineno;
  char* filename;
  struct lex_pushed_context* next;
};

#define ASSEMBLY_SIZE PATH_MAX
struct lex_context {
  FILE *input;
  char assembly[ASSEMBLY_SIZE];
  int lineno;
  char* filename;
  struct lex_pushed_context* last;
};

struct lex_context *lex_file(const char *filename) {
  FILE *in = fopen(filename, "r");
  if (!in) {
    perror(filename);
  }
  return lex_FILE(in, filename);
}

struct lex_context *lex_FILE(FILE* file, const char* name) {
  struct lex_context *ctx = calloc(1, sizeof(struct lex_context));
  ctx->input = file;
  ctx->lineno = 1;
  if (name) {
    ctx->filename = strdup(name);
  } else {
    ctx->filename = strdup("{stream}");
  }
  return ctx;
}

void lex_complete(struct lex_context *ctx) {
  fclose(ctx->input);
  free(ctx->filename);
  free(ctx);
}

struct token *lex_scan(struct lex_context *ctx) {
  int c;

  while (!feof(ctx->input)) {
    c = fgetc(ctx->input);
    switch (c) {
    case EOF:
      return lex_end_of_file(ctx);
    case ',':
      return new_token(ctx->lineno, ctx->filename, COMMA, ",");
    case ';':
      return new_token(ctx->lineno, ctx->filename, SEMICOLON, ";");
    case ':':
      return new_token(ctx->lineno, ctx->filename, COLON, ":");
    case '(':
      return new_token(ctx->lineno, ctx->filename, LPAREN, "(");
    case ')':
      return new_token(ctx->lineno, ctx->filename, RPAREN, ")");
    case '\'':
    case '\"':
      ungetc(c, ctx->input);
      return lex_quotedstring(ctx);
    case ' ':
    case '\t':
      continue;
    case '\n':
      ctx->lineno++;
      continue;
    case '-':
    case '+':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      ungetc(c, ctx->input);
      return lex_number(ctx);
    default:

      if (isalpha(c)) {
        ungetc(c, ctx->input);
        return lex_word(ctx);
      }

      fprintf(stderr, "Unknown value in input: \'%c\'\n", c);
      break;
    }
  }

  /* At end of input, no tokens */
  return lex_end_of_file(ctx);
}

struct token *lex_word(struct lex_context *ctx) {
  memset(ctx->assembly, 0, ASSEMBLY_SIZE);
  int c;
  for (int i = 0; i < ASSEMBLY_SIZE; ++i) {
    c = fgetc(ctx->input);
    if (isalpha(c) || '-' == c) {
      ctx->assembly[i] = c;
    } else {
      ungetc(c, ctx->input);
      break;
    }
  }

  if (strcasecmp(ctx->assembly, "use") == 0) {
    for(;;) {
      c = fgetc(ctx->input);
      if (c == ' ') continue;
      if (c == '\t') continue;
      if (c == '\n') {
        lex_error(ctx, "No filename on USE line");
      }
      if (c == '\'' || c == '\"') {
        ungetc(c, ctx->input);
        break;
      }
    }
    struct token* filename = lex_quotedstring(ctx);
    return lex_use_file(ctx, filename->strval);
  }

  for (int i = 0; i < sizeof(keywords) / sizeof(struct fixed_value); ++i) {
    if (strcasecmp(ctx->assembly, keywords[i].text) == 0)
      return new_token(ctx->lineno, ctx->filename, keywords[i].token_type, ctx->assembly);
  }

  for (int i = 0; i < sizeof(attributes) / sizeof(struct fixed_value); ++i) {
    if (strcasecmp(ctx->assembly, attributes[i].text) == 0)
      return new_token_attribute(ctx->lineno, ctx->filename, attributes[i].token_type,
                                 ctx->assembly);
  }

  return new_token(ctx->lineno, ctx->filename, WORD, ctx->assembly);
}

struct token* lex_quotedstring(struct lex_context* ctx) {
  char delim = fgetc(ctx->input);
  int startline = ctx->lineno;
  memset(ctx->assembly, 0, ASSEMBLY_SIZE);

  int i=0;
  for(int c = fgetc(ctx->input); i < ASSEMBLY_SIZE; c = fgetc(ctx->input)) {
    switch(c) {
      case EOF:
        lex_error(ctx, "End of File while reading quoted string");
        return new_token(startline, ctx->filename, QUOTEDSTRING, ctx->assembly);
      case '\n':
        ctx->lineno++;
        return new_token(startline, ctx->filename, QUOTEDSTRING, ctx->assembly);
        break;
      default:
        if (delim == c) {
          return new_token(startline, ctx->filename, QUOTEDSTRING, ctx->assembly);
        }
        ctx->assembly[i++] = c;
        break;

    }
  }

  lex_error(ctx, "Quoted String longer than max allowed size");
  int idx;
  while(idx = fgetc(ctx->input)) {
    if (idx == EOF)  {     
      lex_error(ctx, "End of File while reading quoted string");
      return new_token(startline, ctx->filename, QUOTEDSTRING, ctx->assembly);
    }
    if (idx == '\n') ctx->lineno++;
    if (idx == delim) {
      return new_token(startline, ctx->filename, QUOTEDSTRING, ctx->assembly);
    }
  }
}

struct token *lex_number(struct lex_context *ctx) {
  memset(ctx->assembly, 0, ASSEMBLY_SIZE);
  int i=0;
  int c;
  ctx->assembly[i++] = fgetc(ctx->input);
  for(c = fgetc(ctx->input); c != EOF && i < ASSEMBLY_SIZE && isdigit(c); c = fgetc(ctx->input)) {
    ctx->assembly[i++] = c;
  }
  if (i < ASSEMBLY_SIZE && c != EOF) {
      ungetc(c, ctx->input);
  }
  struct token* tok = new_token_int(ctx->lineno, ctx->filename, strtol(ctx->assembly, NULL, 10), ctx->assembly);
  return tok;
}


struct token* lex_end_of_file(struct lex_context* ctx) {
  if (ctx->last) {
    struct lex_pushed_context* last = ctx->last;
    fclose(ctx->input);
    ctx->input = last->input;
    ctx->filename = last->filename;
    ctx->lineno = last->lineno;
    ctx->last = last->next;
    free(last);
    return lex_scan(ctx);
  }
  return NULL;
}

struct token* lex_use_file(struct lex_context* ctx, char* filename) {
 
  FILE *newinput = fopen(filename, "r");
  if (!newinput) {
    perror(filename);
    return lex_scan(ctx);
  }

  struct lex_pushed_context* pushed = (struct lex_pushed_context*)calloc(1, sizeof(struct lex_pushed_context));
  pushed->filename = ctx->filename;
  pushed->input = ctx->input;
  pushed->lineno = ctx->lineno;

  ctx->last = pushed;
  ctx->filename = filename;
  ctx->input = newinput;
  ctx->lineno = 1;

  return lex_scan(ctx);

}

void lex_error(struct lex_context* ctx, const char* message) {
  fprintf(stderr, "%s line %d: %s\n", ctx->filename, ctx->lineno, message);
}