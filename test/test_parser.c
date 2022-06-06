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
    struct world_t* w = parse_buffer("skill \"Ranged (Heavy)\" (agility)");

    struct skill_t* s = world_find_skill(w, "Ranged (Heavy)");

    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_NOT_NULL(s->name);
    TEST_ASSERT_EQUAL_STRING(SKILL_NAME, s->name);
    TEST_ASSERT_EQUAL_INT(attr_agility, s->attribute);
}

TEST(Parser, Talent_in_input_becomes_part_of_world) {
    struct world_t* w = parse_buffer("talent Born With It");

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

TEST_GROUP_RUNNER(Parser) {
    RUN_TEST_CASE(Parser, Skill_in_input_becomes_part_of_world);
    RUN_TEST_CASE(Parser, Talent_in_input_becomes_part_of_world);
    RUN_TEST_CASE(Parser, Skill_and_talent_in_input_becomes_part_of_world);
    RUN_TEST_CASE(Parser, Two_Skills_Present_Both_Appear_In_World);
}