#include <gtest/gtest.h>

extern "C" {
  #include "ast.h"
}

TEST(Ast, new_world_creates_null_initialized_word) {
    struct world_t* world = new_world();

    EXPECT_EQ(nullptr, world->skills);
    EXPECT_EQ(nullptr, world->talents);
    EXPECT_EQ(nullptr, world->packages);
    EXPECT_EQ(nullptr, world->characters);
}