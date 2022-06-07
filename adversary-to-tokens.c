#include "adversary.h"
#include "terminal_tags.h"
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

  yyin = fopen(argv[1], "r");
  if (!yyin) {
    perror(argv[1]);
    return EXIT_FAILURE;
  }

  int yymajor;
  while ((yymajor = yylex())) {
    if (yymajor == QUOTEDSTRING) {
      printf("%s(%s)\n", tag_to_name(yymajor), quoted_string);
    } else {
      printf("%s(%s)\n", tag_to_name(yymajor), yytext);
    }
  }

  return EXIT_SUCCESS;
}