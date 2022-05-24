#include <gtest/gtest.h>
#include <tuple>

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
    ASSERT_EQ(256, tok->intval);
}