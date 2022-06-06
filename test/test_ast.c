#include <stdlib.h>

#include "unity_fixture.h"

#include "ast.h"

TEST_GROUP(AST);

TEST_SETUP(AST) {}

TEST_TEAR_DOWN(AST) {}

TEST(AST, is_skill_returns_true_when_given_skill) {
  struct skill_t *s = new_skill("Target", attr_agility);
  TEST_ASSERT_TRUE(is_skill((void *)s, "Target"));
}

TEST(AST, is_skill_returns_false_when_given_not_skill) {
  struct skill_t *s = new_skill("Not It", attr_agility);
  TEST_ASSERT_FALSE(is_skill((void *)s, "Something Else"));
}

TEST(AST, new_world_creates_null_initialized_word) {
  struct world_t *world = new_world();

  TEST_ASSERT_NULL(world->skills);
  TEST_ASSERT_NULL(world->talents);
  TEST_ASSERT_NULL(world->packages);
  TEST_ASSERT_NULL(world->characters);
}

TEST(AST, node_append_adds_node_to_empty_list) {
  struct node_t *TOP = NULL;
  struct skill_t *s = new_skill("Ranged (Light)", attr_agility);
  TOP = node_append(TOP, (void *)s);

  TEST_ASSERT_EQUAL_PTR((void *)s, TOP->node);
}

TEST(AST, node_find_when_item_in_list_returns_node) {
  struct node_t *TOP = NULL;
  struct skill_t *s = new_skill("Ranged (Light)", attr_agility);
  TOP = node_append(TOP, (void *)s);

  TEST_ASSERT_EQUAL_PTR((void *)s, node_find(TOP, "Ranged (Light)", is_skill));
}

TEST(AST, node_find_when_item_not_in_list_returns_null) {
  struct node_t *TOP = NULL;
  struct skill_t *s = new_skill("Ranged (Light)", attr_agility);
  TOP = node_append(TOP, (void *)s);

  TEST_ASSERT_NULL(node_find(TOP, "Ranged (Heavy)", is_skill));
}

TEST(AST, is_namedlist_given_matching_list_returns_true) {
  struct namedlist_t *lst = new_namedlist(list_package, "Sample");
  TEST_ASSERT_TRUE(is_namedlist((void *)lst, "Sample"));
}

TEST(AST, is_namedlist_given_mismatched_list_returns_false) {
  struct namedlist_t *lst = new_namedlist(list_package, "Not A Sample");
  TEST_ASSERT_FALSE(is_namedlist((void *)lst, "Sample"));
}

TEST(AST, node_append_adds_node_to_populated_list) {
  struct node_t *TOP = NULL;
  struct skill_t *s = new_skill("Ranged (Light)", attr_agility);
  struct skill_t *b = new_skill("Brawling", attr_brawn);
  TOP = node_append(TOP, (void *)s);
  TOP = node_append(TOP, (void *)b);

  TEST_ASSERT_EQUAL_PTR((void *)b, node_find(TOP, "Brawling", is_skill));
}

TEST(AST, world_add_skill_adds_skill_to_world) {
  const char *SKILL_NAME = "Skulldugery";
  struct world_t *world = new_world();
  struct skill_t *sk = new_skill(SKILL_NAME, attr_cunning);

  world_add_skill(world, sk);
  TEST_ASSERT_EQUAL_PTR(sk, world_find_skill(world, SKILL_NAME));
}

TEST(AST, world_returns_null_when_skill_not_in_world) {
  const char *SKILL_NAME = "Skulldugery";
  struct world_t *w = new_world();

  TEST_ASSERT_NULL(world_find_skill(w, SKILL_NAME));
}

TEST(AST, world_add_talent_adds_talent_to_world) {
  const char *TALENT_NAME = "Born With It";
  struct world_t *world = new_world();
  struct talent_t *t = new_talent(TALENT_NAME);

  world_add_talent(world, t);

  TEST_ASSERT_EQUAL_PTR(t, world_find_talent(world, TALENT_NAME));
}

TEST(AST, world_add_pakcage_adds_package_to_world) {
  const char *PACKAGE_NAME = "Gang Heavy";
  struct world_t *world = new_world();
  struct namedlist_t *nl = new_namedlist(list_package, PACKAGE_NAME);

  world_add_package(world, nl);

  TEST_ASSERT_EQUAL_PTR(nl, world_find_package(world, PACKAGE_NAME));
}

TEST(AST, world_add_character_adds_character_to_world) {
  const char *CHARACTER_NAME = "3rd Street Gang Muscle";
  struct world_t *world = new_world();
  struct namedlist_t *nl = new_namedlist(list_minion, CHARACTER_NAME);

  world_add_character(world, nl);

  TEST_ASSERT_EQUAL_PTR(nl, world_find_character(world, CHARACTER_NAME));
}

TEST(AST, world_add_reference_given_skill_provides_skill_reference) {
  const char *SKILL_NAME = "Computers";
  struct skill_t *s = new_skill(SKILL_NAME, attr_intellect);
  struct world_t *world = new_world();
  world_add_skill(world, s);

  struct listitem_t *li = world_add_reference(world, SKILL_NAME, 1);

  TEST_ASSERT_NOT_NULL(li);
  TEST_ASSERT_EQUAL_INT(li_skillref, li->type);

  const struct skill_reference_t *sr = li->skill;
  TEST_ASSERT_EQUAL_PTR(s, sr->reference);
  TEST_ASSERT_EQUAL_INT(1, sr->level);
}

TEST(AST, world_add_reference_given_talent_provides_talent_reference) {
  const char *TALENT_NAME = "Night Vision";
  struct talent_t *t = new_talent(TALENT_NAME);
  struct world_t *world = new_world();
  world_add_talent(world, t);

  struct listitem_t *li = world_add_reference(world, TALENT_NAME, 0);

  TEST_ASSERT_NOT_NULL(li);
  TEST_ASSERT_EQUAL_INT(li_talentref, li->type);

  const struct talent_reference_t *sr = li->talent;
  TEST_ASSERT_EQUAL_PTR(t, sr->reference);
  TEST_ASSERT_EQUAL_INT(0, sr->level);
}

TEST(AST, world_add_reference_given_neither_talent_nor_reference_returns_null) {
  const char *TALENT_NAME = "Night Vision";
  struct talent_t *t = new_talent(TALENT_NAME);
  struct world_t *world = new_world();
  world_add_talent(world, t);

  struct listitem_t *li = world_add_reference(world, "Something Else", 0);

  TEST_ASSERT_NULL(li);
}

TEST_GROUP_RUNNER(AST) {
  RUN_TEST_CASE(AST, is_skill_returns_true_when_given_skill);
  RUN_TEST_CASE(AST, is_skill_returns_false_when_given_not_skill);
  RUN_TEST_CASE(AST, new_world_creates_null_initialized_word);
  RUN_TEST_CASE(AST, node_append_adds_node_to_empty_list);
  RUN_TEST_CASE(AST, node_find_when_item_in_list_returns_node);
  RUN_TEST_CASE(AST, node_find_when_item_not_in_list_returns_null);
  RUN_TEST_CASE(AST, is_namedlist_given_matching_list_returns_true);
  RUN_TEST_CASE(AST, is_namedlist_given_mismatched_list_returns_false);
  RUN_TEST_CASE(AST, node_append_adds_node_to_populated_list);
  RUN_TEST_CASE(AST, world_add_skill_adds_skill_to_world);
  RUN_TEST_CASE(AST, world_returns_null_when_skill_not_in_world);
  RUN_TEST_CASE(AST, world_add_talent_adds_talent_to_world);
  RUN_TEST_CASE(AST, world_add_pakcage_adds_package_to_world);
  RUN_TEST_CASE(AST, world_add_character_adds_character_to_world);
  RUN_TEST_CASE(AST, world_add_reference_given_skill_provides_skill_reference);
  RUN_TEST_CASE(AST,
                world_add_reference_given_talent_provides_talent_reference);
  RUN_TEST_CASE(
      AST, world_add_reference_given_neither_talent_nor_reference_returns_null);
}