#include <gtest/gtest.h>

extern "C" {
  #include "ast.h"
}

TEST(Ast, is_skill_returns_true_when_given_skill) {
  struct skill_t* s = new_skill("Target", attr_agility);
  EXPECT_TRUE(is_skill((void*)s, "Target"));
}

TEST(Ast, is_skill_returns_false_when_given_not_skill) {
  struct skill_t* s = new_skill("Not It", attr_agility);
  EXPECT_FALSE(is_skill((void*)s, "Something Else"));
}


TEST(Ast, new_world_creates_null_initialized_word) {
    struct world_t* world = new_world();

    EXPECT_EQ(nullptr, world->skills);
    EXPECT_EQ(nullptr, world->talents);
    EXPECT_EQ(nullptr, world->packages);
    EXPECT_EQ(nullptr, world->characters);
}