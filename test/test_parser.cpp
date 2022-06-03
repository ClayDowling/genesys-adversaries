//
// Created by Clay on 5/27/2022.
//

#include <gtest/gtest.h>
#include "../World.h"

extern "C" {
#include "../ast.h"
}

#include <sstream>
using std::istringstream;

TEST(Parser, Skill_in_input_becomes_part_of_world) {
    istringstream in("skill \"Ranged (Heavy)\" (agility)");
    World w;
    w.read(in);

    auto s = world_find_skill(w.world, "Ranged (Heavy)");

    ASSERT_NE(nullptr, s);
    ASSERT_NE(nullptr, s->name);
    EXPECT_EQ(string("Ranged (Heavy)"), s->name);
    EXPECT_EQ(attr_agility, s->attribute);
}

TEST(Parser, Talent_in_input_becomes_part_of_world) {
    istringstream in("talent \"Born With It\"");
    World w;
    w.read(in);

    auto t = world_find_talent(w.world, "Born With It");

    ASSERT_NE(nullptr, t);
    ASSERT_NE(nullptr, t->name);
    ASSERT_EQ(string("Born With It"), t->name);
}

TEST(Parser, Skill_and_talent_in_input_becomes_part_of_world) {
    // istringstream in("talent \"Born With It\"\nskill \"Brawling\" (Brawn)");
   istringstream in("skill \"Brawling\" (Brawn)\ntalent \"Born With It\"\n");
    World w;
    w.read(in);

    auto s = world_find_skill(w.world, "Brawling");
    auto t = world_find_talent(w.world, "Born With It");

    EXPECT_NE(nullptr, s);
    EXPECT_NE(nullptr, t);
}

TEST(Parser, Two_Skills_Present_Both_Appear_In_World) {
    istringstream in("skill \"Brawling\" (Brawn)\nSkill \"Computers\" (Intellect)");
    World w;
    w.read(in);

    auto s1 = world_find_skill(w.world, "Brawling");
    auto s2 = world_find_skill(w.world, "Computers");
    
    EXPECT_NE(nullptr, s1);
    EXPECT_NE(nullptr, s2);
}