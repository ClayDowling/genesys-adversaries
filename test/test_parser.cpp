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
    ASSERT_EQ(string("Ranged (Heavy)"), s->name);
    ASSERT_EQ(attr_agility, s->attribute);
}