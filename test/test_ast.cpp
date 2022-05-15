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

TEST(Ast, is_namedlist_given_matching_list_returns_true) {
  struct namedlist_t* lst = new_namedlist(list_package, "Sample");
  EXPECT_TRUE(is_namedlist((void*)lst, "Sample"));
}

TEST(Ast, is_namedlist_given_mismatched_list_returns_false) {
  struct namedlist_t* lst = new_namedlist(list_package, "Not A Sample");
  EXPECT_FALSE(is_namedlist((void*)lst, "Sample"));
}

TEST(Ast, node_append_adds_node_to_populated_list) {
  struct node_t *TOP = nullptr;
  struct skill_t* s = new_skill("Ranged (Light)", attr_agility);
  struct skill_t* b = new_skill("Brawling", attr_brawn);
  TOP = node_append(TOP, (void*)s);
  TOP = node_append(TOP, (void*)b);

  EXPECT_EQ((void*)b, node_find(TOP, "Brawling", is_skill));
}

TEST(Ast, world_add_skill_adds_skill_to_world) {
  const char* SKILL_NAME = "Skulldugery";
  struct world_t* world = new_world();
  struct skill_t* sk = new_skill(SKILL_NAME, attr_cunning);
  
  world_add_skill(world, sk);
  EXPECT_EQ(sk, world_find_skill(world, SKILL_NAME));
}

TEST(Ast, world_add_talent_adds_talent_to_world) {
  const char* TALENT_NAME = "Born With It";
  struct world_t* world = new_world();
  struct talent_t* t = new_talent(TALENT_NAME);
  
  world_add_talent(world, t);
 
  EXPECT_EQ(t, world_find_talent(world, TALENT_NAME));
}

TEST(Ast, world_add_pakcage_adds_package_to_world) {
  const char* PACKAGE_NAME = "Gang Heavy";
  struct world_t* world = new_world();
  struct namedlist_t* nl = new_namedlist(list_package, PACKAGE_NAME);
  
  world_add_package(world, nl);
 
  EXPECT_EQ(nl, world_find_package(world, PACKAGE_NAME));
}

TEST(Ast, world_add_character_adds_character_to_world) {
  const char* CHARACTER_NAME = "3rd Street Gang Muscle";
  struct world_t* world = new_world();
  struct namedlist_t* nl = new_namedlist(list_minion, CHARACTER_NAME);
  
  world_add_character(world, nl);
 
  EXPECT_EQ(nl, world_find_character(world, CHARACTER_NAME));
}

TEST(Ast, world_add_reference_given_skill_provides_skill_reference) {
  const char* SKILL_NAME = "Computers";
  struct skill_t* s = new_skill(SKILL_NAME, attr_intellect);
  struct world_t* world = new_world();
  world_add_skill(world, s);
  
  struct listitem_t* li = world_add_reference(world, SKILL_NAME, 1);

  ASSERT_NE(nullptr, li);
  ASSERT_EQ(li_skillref, li->type);
  
  const struct skill_reference_t* sr = li->skill;
  EXPECT_EQ(s, sr->reference);
  EXPECT_EQ(1, sr->level);

}

TEST(Ast, world_add_reference_given_talent_provides_talent_reference) {
  const char* TALENT_NAME = "Night Vision";
  struct talent_t* t = new_talent(TALENT_NAME);
  struct world_t* world = new_world();
  world_add_talent(world, t);
  
  struct listitem_t* li = world_add_reference(world, TALENT_NAME, 0);

  ASSERT_NE(nullptr, li);
  ASSERT_EQ(li_talentref, li->type);
  
  const struct talent_reference_t* sr = li->talent;
  EXPECT_EQ(t, sr->reference);
  EXPECT_EQ(0, sr->level);
}

TEST(Ast, world_add_reference_given_neither_talent_nor_reference_returns_null) {
  const char* TALENT_NAME = "Night Vision";
  struct talent_t* t = new_talent(TALENT_NAME);
  struct world_t* world = new_world();
  world_add_talent(world, t);
  
  struct listitem_t* li = world_add_reference(world, "Something Else", 0);

  ASSERT_EQ(nullptr, li);
}