#include <gtest/gtest.h>
#include <tuple>
#include <string>
using std::string;

extern "C" {
#include "lexer.h"
}

class TokenTest : public testing::TestWithParam<std::tuple<int, const char*> > {

};


TEST_P(TokenTest, StringGeneratesExpectedToken) {

    auto instance = GetParam();

    int expected = std::get<0>(instance);
    const char* str = std::get<1>(instance);

    auto tok = lex(str);
    ASSERT_EQ(expected, tok->token_type);
}

INSTANTIATE_TEST_SUITE_P(SingleTokens, TokenTest, 
    testing::Values(
        std::make_tuple(SKILL, "Skill"),
        std::make_tuple(TALENT, "Talent"),
        std::make_tuple(PACKAGE, "Package"),
        std::make_tuple(MINION, "Minion"),
        std::make_tuple(RIVAL, "Rival"),
        std::make_tuple(NEMESIS, "Nemesis"),
        std::make_tuple(COLON, ":"),
        std::make_tuple(COMMA, ","),
        std::make_tuple(LPAREN, "("),
        std::make_tuple(RPAREN, ")")
    )
);


class LexerAttribute : public testing::TestWithParam<std::tuple<int, const char*> > {

};


TEST_P(LexerAttribute, StringGeneratesExpectedAttribute) {

    auto instance = GetParam();

    int expected = std::get<0>(instance);
    const char* str = std::get<1>(instance);

    auto tok = lex(str);
    ASSERT_EQ(ATTRIBUTE, tok->token_type);
    ASSERT_EQ(expected, tok->attributeval);
}

INSTANTIATE_TEST_SUITE_P(AttributeTokens, LexerAttribute, 
    testing::Values(
        std::make_tuple(attr_brawn, "Brawn"),
        std::make_tuple(attr_brawn, "BR"),
        std::make_tuple(attr_agility, "Agility"),
        std::make_tuple(attr_agility, "Ag"),
        std::make_tuple(attr_intellect, "Intellect"),
        std::make_tuple(attr_intellect, "Int"),
        std::make_tuple(attr_cunning, "Cunning"),
        std::make_tuple(attr_cunning, "Cun"),
        std::make_tuple(attr_willpower, "Willpower"),
        std::make_tuple(attr_willpower, "WILL"),
        std::make_tuple(attr_presence, "Presence"),
        std::make_tuple(attr_presence, "Pr")
    )
);

TEST(Lexer, givenEmptyString_ReturnsNullPtr) {

    ASSERT_EQ(nullptr, lex(""));

}

TEST(Lexer, givenNumber_ReturnsCorrectValue) {
    auto tok = lex("256");
    ASSERT_EQ(NUMBER, tok->token_type);
    ASSERT_EQ(256, tok->intval);
}

TEST(Lexer, givenNegativeNumber_ReturnsCorrectValue) {
    auto tok = lex("-7");
    ASSERT_EQ(NUMBER, tok->token_type);
    ASSERT_EQ(-7, tok->intval);
}

TEST(Lexer, givenName_ReturnsExpectedName) {
    auto tok = lex("\"Ranged (Light)\"");
    ASSERT_EQ(NAME, tok->token_type);
    ASSERT_EQ(string("Ranged (Light)"), string(tok->strval));
}

TEST(Lexer, givenEmptyName_ReturnsEmptyName) {
    auto tok = lex("\"\"");
    ASSERT_EQ(NAME, tok->token_type);
    ASSERT_EQ(string(""), string(tok->strval));
}

TEST(Lexer, givenMultipleInputLines_ReturnsCorrectLineNumber) {
    auto tok = lex("\nskill");
    ASSERT_EQ(SKILL, tok->token_type);
    ASSERT_EQ(2, tok->lineno);
}