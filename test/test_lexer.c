#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adversary.h"
#include "lexer.h"
#include "terminal_tags.h"
#include "unity_fixture.h"

char *tmpfilename = NULL;
struct lex_context *ctx = NULL;

TEST_GROUP(Lexer);

TEST_SETUP(Lexer) {
  tmpfilename = NULL;
  ctx = NULL;
}

TEST_TEAR_DOWN(Lexer) {
  if (tmpfilename) {
    // remove(tmpfilename);
    // free(tmpfilename);
    tmpfilename = NULL;
  }
  if (ctx) {
    lex_complete(ctx);
    ctx = NULL;
  }
}

void useContent(const char *content) {
  char template[] = "lexerXXXXXX";
  //   tmpfilename = strdup(template);
  FILE *out = fdopen(mkstemp(template), "w+b");
  fwrite((void *)content, strlen(content), 1, out);
  fflush(out);
  rewind(out);

  ctx = lex_file(template);
}

const char *mismatch_message(int expected, int actual) {
  static char message[100];
  snprintf(message, sizeof(message), "Expected %s but found %s",
           tag_to_name(expected), tag_to_name(actual));
  return message;
}

TEST(Lexer, single_character_tokens_return_expected_values) {
  int expected[] = {COMMA, COLON, LPAREN, RPAREN, 0};
  struct token *t;
  useContent(", : ( )");
  int matches = 0;

  for (int i = 0; expected[i]; ++i) {
    t = lex_scan(ctx);
    if (!t)
      break;
    TEST_ASSERT_EQUAL_INT_MESSAGE(expected[i], t->token_type,
                                  mismatch_message(expected[i], t->token_type));
    matches++;
  }

  TEST_ASSERT_EQUAL_INT(4, matches);
}

TEST(Lexer, keywords_return_expected_values) {
  int expected[] = {SKILL, TALENT, PACKAGE, MINION, RIVAL, NEMESIS};
  struct token *t;
  useContent("skill talent package minion rival nemesis");
  int matches = 0;

  for (int i = 0; expected[i]; ++i) {
    t = lex_scan(ctx);
    if (!t)
      break;
    TEST_ASSERT_EQUAL_INT_MESSAGE(expected[i], t->token_type,
                                  mismatch_message(expected[i], t->token_type));
    matches++;
  }

  TEST_ASSERT_EQUAL_INT(6, matches);
}

TEST(Lexer, keyword_matches_are_case_insensitive) {
  int expected[] = {SKILL, TALENT, PACKAGE, MINION, RIVAL, NEMESIS};
  struct token *t;
  useContent("Skill talenT package MINION Rival nEmeSis");
  int matches = 0;

  for (int i = 0; expected[i]; ++i) {
    t = lex_scan(ctx);
    if (!t)
      break;
    TEST_ASSERT_EQUAL_INT_MESSAGE(expected[i], t->token_type,
                                  mismatch_message(expected[i], t->token_type));
    matches++;
  }

  TEST_ASSERT_EQUAL_INT_MESSAGE(6, matches, "Unmatched keywords");
}

TEST_GROUP_RUNNER(Lexer) {
  RUN_TEST_CASE(Lexer, single_character_tokens_return_expected_values);
  RUN_TEST_CASE(Lexer, keywords_return_expected_values);
  RUN_TEST_CASE(Lexer, keyword_matches_are_case_insensitive);
}