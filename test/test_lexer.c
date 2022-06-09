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

TEST(Lexer, attribute_matches_in_case_insensitive_way) {
  enum attribute_t expected[] = {attr_brawn,     attr_agility,   attr_cunning,
                                 attr_intellect, attr_willpower, attr_presence};
  struct token *t;
  useContent("brawn Agility cUnNiNg Intellect willPOWER PRESENCE");
  int matches = 0;

  for (int i = 0; i < sizeof(expected) / sizeof(enum attribute_t); ++i) {
    t = lex_scan(ctx);
    if (!t)
      break;
    TEST_ASSERT_EQUAL_INT_MESSAGE(ATTRIBUTE, t->token_type,
                                  mismatch_message(ATTRIBUTE, t->token_type));
    TEST_ASSERT_EQUAL_INT((int)expected[i], (int)t->attributeval);
    matches++;
  }
  TEST_ASSERT_EQUAL_INT(6, matches);
}

TEST(Lexer, attribute_abbreviations_matche_in_case_insensitive_way) {
  enum attribute_t expected[] = {attr_brawn,     attr_agility,   attr_cunning,
                                 attr_intellect, attr_willpower, attr_presence};
  struct token *t;
  useContent("br Ag CUN inT wIll PR");
  int matches = 0;

  for (int i = 0; i < sizeof(expected) / sizeof(enum attribute_t); ++i) {
    t = lex_scan(ctx);
    if (!t)
      break;
    TEST_ASSERT_EQUAL_INT_MESSAGE(ATTRIBUTE, t->token_type,
                                  mismatch_message(ATTRIBUTE, t->token_type));
    TEST_ASSERT_EQUAL_INT((int)expected[i], (int)t->attributeval);
    matches++;
  }
  TEST_ASSERT_EQUAL_INT(6, matches);
}

TEST(Lexer, non_reserved_words_return_word_token) {
  struct token *first;
  struct token *second;
  useContent("Snow White");

  first = lex_scan(ctx);
  TEST_ASSERT_NOT_NULL(first);
  TEST_ASSERT_EQUAL_INT_MESSAGE(WORD, first->token_type,
                                mismatch_message(WORD, first->token_type));
  TEST_ASSERT_EQUAL_STRING("Snow", first->strval);
  destroy_token(first);

  second = lex_scan(ctx);
  TEST_ASSERT_NOT_NULL(second);
  TEST_ASSERT_EQUAL_INT_MESSAGE(WORD, second->token_type,
                                mismatch_message(WORD, second->token_type));
  TEST_ASSERT_EQUAL_STRING("White", second->strval);
  destroy_token(second);
}

TEST_GROUP_RUNNER(Lexer) {
  RUN_TEST_CASE(Lexer, single_character_tokens_return_expected_values);
  RUN_TEST_CASE(Lexer, keywords_return_expected_values);
  RUN_TEST_CASE(Lexer, keyword_matches_are_case_insensitive);
  RUN_TEST_CASE(Lexer, attribute_matches_in_case_insensitive_way);
  RUN_TEST_CASE(Lexer, attribute_abbreviations_matche_in_case_insensitive_way);
  RUN_TEST_CASE(Lexer, non_reserved_words_return_word_token);
}