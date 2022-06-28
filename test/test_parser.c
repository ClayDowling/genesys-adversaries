//
// Created by Clay on 5/27/2022.
//

#include "unity_fixture.h"
#include "../ast.h"
#include "../parser.h"

TEST_GROUP(Parser);

TEST_SETUP(Parser) {}

TEST_TEAR_DOWN(Parser) {}

TEST(Parser, Skill_in_input_becomes_part_of_world) {
    const char *SKILL_NAME = "Ranged (Heavy)";
    struct world_t* w = parse_buffer("skill \"Ranged (Heavy)\" (agility)\nSkill \"Stuff and nonsense\" (cun)\n");

    struct skill_t* s = world_find_skill(w, "Ranged (Heavy)");

    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_NOT_NULL(s->name);
    TEST_ASSERT_EQUAL_STRING(SKILL_NAME, s->name);
    TEST_ASSERT_EQUAL_INT(attr_agility, s->attribute);
}

TEST(Parser, Talent_in_input_becomes_part_of_world) {
    struct world_t* w = parse_buffer("talent \"Born With It\"");

    struct talent_t* t = world_find_talent(w, "Born With It");

    TEST_ASSERT_NOT_NULL( t);
    TEST_ASSERT_NOT_NULL( t->name);
    TEST_ASSERT_EQUAL_STRING("Born With It", t->name);
}

TEST(Parser, Skill_and_talent_in_input_becomes_part_of_world) {
   
    struct world_t* w = parse_buffer("Skill Brawling (Brawn)\ntalent \"Born With It\"");

    struct skill_t* s = world_find_skill(w, "Brawling");
    struct talent_t* t = world_find_talent(w, "Born With It");

    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_NOT_NULL(t);
}

TEST(Parser, Two_Skills_Present_Both_Appear_In_World) {

    struct world_t* w = parse_buffer("skill Brawling (Brawn)\nSkill Computers (Int)");

    struct skill_t* s1 = world_find_skill(w, "Brawling");
    struct skill_t* s2 = world_find_skill(w, "Computers");
    
    TEST_ASSERT_NOT_NULL(s1);
    TEST_ASSERT_NOT_NULL(s2);
}

TEST(Parser, Package_is_added_to_world) {
    struct world_t* w = parse_buffer("skill Brawling (Brawn)\n"
                                     "talent Sneaky\n"
                                     "package \"Sneaky Dude\": sneaky, brawling, Br +3\n"
            );

    struct namedlist_t *p = world_find_package(w, "Sneaky Dude");

    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL_INT(list_package, p->type);

    struct listitem_t *brawling = node_find(p->TOP, "brawling", is_skill_reference_name);
    TEST_ASSERT_NOT_NULL(brawling);
    TEST_ASSERT_EQUAL_INT(li_skillref, brawling->type);
    TEST_ASSERT_EQUAL_INT(1, brawling->skill->level);

    struct listitem_t *sneaky = node_find(p->TOP, "sneaky", is_talent_reference_name);
    TEST_ASSERT_NOT_NULL(sneaky);
    TEST_ASSERT_EQUAL_INT(li_talentref, sneaky->type);
    TEST_ASSERT_EQUAL_INT(0, sneaky->talent->level);
}

TEST(Parser, Character_is_added_to_world) {
    struct world_t* w = parse_buffer("skill Melee (Brawn)\n"
                                     "talent Grit\n"
                                     "rival \"Henchman\": melee, grit 2, Will +2\n"
    );

    struct namedlist_t *r = world_find_character(w, "Henchman");

    TEST_ASSERT_NOT_NULL(r);
    TEST_ASSERT_EQUAL_INT(list_rival, r->type);

    struct listitem_t *melee = node_find(r->TOP, "melee", is_skill_reference_name);
    TEST_ASSERT_NOT_NULL(melee);
    TEST_ASSERT_EQUAL_INT(li_skillref, melee->type);
    TEST_ASSERT_EQUAL_INT(1, melee->skill->level);

    struct listitem_t *grit = node_find(r->TOP, "grit", is_talent_reference_name);
    TEST_ASSERT_NOT_NULL(grit);
    TEST_ASSERT_EQUAL_INT(li_talentref, grit->type);
    TEST_ASSERT_EQUAL_INT(2, grit->talent->level);
}

TEST_GROUP_RUNNER(Parser) {
    RUN_TEST_CASE(Parser, Skill_in_input_becomes_part_of_world);
    RUN_TEST_CASE(Parser, Talent_in_input_becomes_part_of_world);
    RUN_TEST_CASE(Parser, Skill_and_talent_in_input_becomes_part_of_world);
    RUN_TEST_CASE(Parser, Two_Skills_Present_Both_Appear_In_World);
    RUN_TEST_CASE(Parser, Package_is_added_to_world);
    RUN_TEST_CASE(Parser, Character_is_added_to_world);
}
