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

TEST(Ast, node_append_adds_node_to_empty_list) {
  struct node_t *TOP = nullptr;
  struct skill_t* s = new_skill("Ranged (Light)", attr_agility);
  TOP = node_append(TOP, (void*)s);

  EXPECT_EQ((void*)s, TOP->node);
}

TEST(Ast, node_find_when_item_in_list_returns_node) {
  struct node_t *TOP = nullptr;
  struct skill_t* s = new_skill("Ranged (Light)", attr_agility);
  TOP = node_append(TOP, (void*)s);

  EXPECT_EQ((void*)s, node_find(TOP, "Ranged (Light)", is_skill));  
}

TEST(Ast, node_find_when_item_not_in_list_returns_null) {
  struct node_t *TOP = nullptr;
  struct skill_t* s = new_skill("Ranged (Light)", attr_agility);
  TOP = node_append(TOP, (void*)s);

  EXPECT_EQ(NULL, node_find(TOP, "Ranged (Heavy)", is_skill));  
}

TEST(Ast, node_append_adds_node_to_populated_list) {
  struct node_t *TOP = nullptr;
  struct skill_t* s = new_skill("Ranged (Light)", attr_agility);
  struct skill_t* b = new_skill("Brawling", attr_brawn);
  TOP = node_append(TOP, (void*)s);
  TOP = node_append(TOP, (void*)b);

  EXPECT_EQ((void*)b, node_find(TOP, "Brawling", is_skill));
}