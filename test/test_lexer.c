#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "adversary.h"
#include "lexer.h"
#include "terminal_tags.h"
#include "unity_fixture.h"

struct lex_context *ctx = NULL;
FILE *srcfile = NULL;
char filename[12];

TEST_GROUP(Lexer);

TEST_SETUP(Lexer) {
  ctx = NULL;
  strncpy(filename, "lexerXXXXXX", sizeof(filename));
}

TEST_TEAR_DOWN(Lexer) {
  if (ctx) {
    lex_complete(ctx);
    ctx = NULL;
  }
  struct stat sb;
  if (stat(filename, &sb) == 0) {  
    remove(filename);
  }
}

void useContent(const char *content) {
  int fd = mkstemp(filename);
  if (fd == -1) {
    perror(filename);
    exit(1);
  }
  if (write(fd, (void*)content, strlen(content)) == -1) {
    perror("write");
    exit(2);
  }
  close(fd);

  ctx = lex_file(filename);
}

const char *mismatch_message(int expected, int actual) {
  static char message[100];
  snprintf(message, sizeof(message), "Expected %s but found %s",
           tag_to_name(expected), tag_to_name(actual));
  return message;
}

const char *attribute_mismatch_message(enum attribute_t expected, enum attribute_t actual) {
    static char message[100];
    snprintf(message, sizeof(message), "Expected attribute %s but found %s",
        attribute_name(expected), attribute_name(actual));
    return message;
}

TEST(Lexer, single_character_tokens_return_expected_values) {
  int expected[] = {COMMA, SEMICOLON, COLON, LPAREN, RPAREN, 0};
  struct token *t;
  useContent(", ; : ( )");
  int matches = 0;

  for (int i = 0; expected[i]; ++i) {
    t = lex_scan(ctx);
    if (!t)
      break;
    TEST_ASSERT_EQUAL_INT_MESSAGE(expected[i], t->token_type,
                                  mismatch_message(expected[i], t->token_type));
    matches++;
  }

  TEST_ASSERT_EQUAL_INT(5, matches);
}

TEST(Lexer, keywords_return_expected_values) {
  int expected[] = {SKILL, TALENT, PACKAGE, MINION, RIVAL, NEMESIS, WEAPON, DAMAGE, DAMAGE, CRITICAL, CRITICAL};
  struct token *t;
  useContent("skill talent package minion rival nemesis weapon damage dmg critical crit");
  int matches = 0;

  for (int i = 0; expected[i]; ++i) {
    t = lex_scan(ctx);
    if (!t)
      break;
    TEST_ASSERT_EQUAL_INT_MESSAGE(expected[i], t->token_type,
                                  mismatch_message(expected[i], t->token_type));
    matches++;
  }

  TEST_ASSERT_EQUAL_INT(sizeof(expected) / sizeof(int), matches);
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

TEST(Lexer, attribute_abbreviations_match_in_case_insensitive_way) {
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

TEST(Lexer, power_attributes_match_in_case_insensitive_way) {
  enum attribute_t expected[] = {
    attr_combat, attr_combat, attr_social, attr_social, attr_general, attr_general
  };
  struct token *t;
  useContent("Combat cbt sOc SoCiAl gen GeneraL");
  int matches = 0;

  for(int i=0; i < sizeof(expected)/ sizeof(enum attribute_t); ++i) {
    t = lex_scan(ctx);
    if (!t) break;
    TEST_ASSERT_EQUAL_INT_MESSAGE(ATTRIBUTE, t->token_type, 
                                  mismatch_message(ATTRIBUTE, t->token_type));
    TEST_ASSERT_EQUAL_INT((int)expected[i], (int)t->attributeval);
    matches++;
  }
  TEST_ASSERT_EQUAL_INT(6, matches);
}

TEST(Lexer, defense_attributes_match_in_case_insensitive_way) {
  enum attribute_t expected[] = {
    attr_meleedefense, attr_meleedefense, 
    attr_rangeddefence, attr_rangeddefence, 
    attr_wound, attr_soak, attr_strain
  };
  struct token *t;
  useContent("meleedefense melee-defense rangeddefense ranged-defense wound soak strain");
  int matches = 0;

  for(int i=0; i < sizeof(expected)/ sizeof(enum attribute_t); ++i) {
    t = lex_scan(ctx);
    if (!t) break;
    TEST_ASSERT_EQUAL_INT_MESSAGE(ATTRIBUTE, t->token_type, 
                                  mismatch_message(ATTRIBUTE, t->token_type));
    TEST_ASSERT_EQUAL_INT_MESSAGE((int)expected[i], (int)t->attributeval,
                                  attribute_mismatch_message(expected[i], t->attributeval));
    matches++;
  }
  TEST_ASSERT_EQUAL_INT(7, matches);
}


TEST(Lexer, non_reserved_words_return_word_token) {
  struct token *first;
  struct token *second;
  struct token *third;
  useContent("Snow White Obi-Wan");

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

  third = lex_scan(ctx);
  TEST_ASSERT_NOT_NULL(third);
  TEST_ASSERT_EQUAL_INT_MESSAGE(WORD, third->token_type, mismatch_message(WORD, third->token_type));
  TEST_ASSERT_EQUAL_STRING("Obi-Wan", third->strval);
  destroy_token(third);
}

TEST(Lexer, quoted_string_returns_QUOTEDSTRING_symbol) {
  struct token* tok;
  useContent("\"Ranged (Light)\"");

  tok = lex_scan(ctx);
  TEST_ASSERT_NOT_NULL(tok);
  TEST_ASSERT_EQUAL_INT(QUOTEDSTRING, tok->token_type);
  TEST_ASSERT_EQUAL_STRING("Ranged (Light)", tok->strval);
  destroy_token(tok);
}

TEST(Lexer, use_keyword_causes_named_file_to_be_read) {
  struct token* first;
  struct token* second;

  char tempfile[10];
  strncpy(tempfile, "useXXXXXX", sizeof(tempfile));
  int fd = mkstemp(tempfile);
  write(fd, "talent\n", 7);
  close(fd);

  char content[256];
  memset(content, 0, sizeof(content));
  snprintf(content, sizeof(content), "use \"%s\"\n"
                                     "skill\n", tempfile);

  useContent(content);

  first = lex_scan(ctx);
  second = lex_scan(ctx);

  TEST_ASSERT_NOT_NULL(first);
  TEST_ASSERT_NOT_NULL(second);
  TEST_ASSERT_EQUAL_INT_MESSAGE(TALENT, first->token_type, mismatch_message(TALENT, first->token_type));
  TEST_ASSERT_EQUAL_STRING(tempfile, first->filename);
  TEST_ASSERT_EQUAL_INT_MESSAGE(SKILL, second->token_type, mismatch_message(SKILL, second->token_type));
  TEST_ASSERT_EQUAL_STRING(filename, second->filename);

  remove(tempfile);
}

TEST(Lexer, use_keyword_when_two_files_deep_loads_all_three_files) {

    char firstfile[10];
    strncpy(firstfile, "useXXXXXX", sizeof(firstfile));
    int fd = mkstemp(firstfile);
    write(fd, "talent\n", 7);
    close(fd);

    char secondfile[10];
    strncpy(secondfile, "useXXXXXX", sizeof(secondfile));
    fd = mkstemp(secondfile);
    char content[256];
    snprintf(content, 256, "use \"%s\"\nskill\n", firstfile);
    write(fd, content, strlen(content));
    close(fd);

    snprintf(content, 256, "use \"%s\"\nbob\n", secondfile);
    useContent(content);

    struct token* first;
    struct token* second;
    struct token* third;

    first = lex_scan(ctx);
    second = lex_scan(ctx);
    third = lex_scan(ctx);

    TEST_ASSERT_NOT_NULL(first);
    TEST_ASSERT_NOT_NULL(second);
    TEST_ASSERT_NOT_NULL(third);

    TEST_ASSERT_EQUAL_INT(TALENT, first->token_type);
    TEST_ASSERT_EQUAL_INT(SKILL, second->token_type);
    TEST_ASSERT_EQUAL_INT(WORD, third->token_type);
}

TEST(Lexer, use_keyword_searches_lex_directories_for_file) {

#ifdef _WIN64
    mkdir("testuse");
#else
    mkdir("testuse", 0755);
#endif

    FILE *out = fopen("testuse/test.adv", "w");
    fputs("skill", out);
    fclose(out);

    lex_add_directory("testuse");
    struct lex_context *ctx = lex_file("test.adv");
    TEST_ASSERT_NOT_NULL(ctx);

    struct token *tok = lex_scan(ctx);

    remove("testuse/test.adv");
    rmdir("testuse");

    TEST_ASSERT_NOT_NULL(tok);
    TEST_ASSERT_EQUAL_INT_MESSAGE(SKILL, tok->token_type, mismatch_message(SKILL, tok->token_type));
}

TEST(Lexer, given_positive_negative_and_signless_numbes_return_correct_values) {
  int expected[] = {128, -256, 7};
  useContent("+128 -256 7");
  int matches = 0;

  struct token* tok;
  for(int i=0; i < 3; ++i) {
    tok = lex_scan(ctx);
    TEST_ASSERT_NOT_NULL(tok);
    TEST_ASSERT_EQUAL_INT_MESSAGE(NUMBER, tok->token_type, mismatch_message(NUMBER, tok->token_type));
    TEST_ASSERT_EQUAL_INT(expected[i], tok->intval);
    matches++;
  }
  TEST_ASSERT_EQUAL_INT(3, matches);
}

TEST(Lexer, given_number_follows_by_comma_returns_both_tokens) {
  useContent("42,");
  struct token *tok1 = lex_scan(ctx);
  struct token *tok2 = lex_scan(ctx);
  struct token *tokeof = lex_scan(ctx);

  TEST_ASSERT_NOT_NULL(tok1);
  TEST_ASSERT_EQUAL_INT(NUMBER, tok1->token_type);
  TEST_ASSERT_NOT_NULL(tok2);
  TEST_ASSERT_EQUAL_INT(COMMA, tok2->token_type);
  TEST_ASSERT_NULL(tokeof);
}

TEST_GROUP_RUNNER(Lexer) {
  RUN_TEST_CASE(Lexer, single_character_tokens_return_expected_values);
  RUN_TEST_CASE(Lexer, keywords_return_expected_values);
  RUN_TEST_CASE(Lexer, keyword_matches_are_case_insensitive);
  RUN_TEST_CASE(Lexer, attribute_matches_in_case_insensitive_way);
  RUN_TEST_CASE(Lexer, attribute_abbreviations_match_in_case_insensitive_way);
  RUN_TEST_CASE(Lexer, power_attributes_match_in_case_insensitive_way);
  RUN_TEST_CASE(Lexer, defense_attributes_match_in_case_insensitive_way);
  RUN_TEST_CASE(Lexer, non_reserved_words_return_word_token);
  RUN_TEST_CASE(Lexer, quoted_string_returns_QUOTEDSTRING_symbol);
  RUN_TEST_CASE(Lexer, use_keyword_causes_named_file_to_be_read);
  RUN_TEST_CASE(Lexer, use_keyword_when_two_files_deep_loads_all_three_files);
  RUN_TEST_CASE(Lexer, use_keyword_searches_lex_directories_for_file);
  RUN_TEST_CASE(Lexer, given_positive_negative_and_signless_numbes_return_correct_values);
  RUN_TEST_CASE(Lexer, given_number_follows_by_comma_returns_both_tokens);
}