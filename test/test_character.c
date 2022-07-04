//
// Created by Clay on 7/2/2022.
//

#include <unity_fixture.h>
#include "ast.h"
#include "character.h"

struct namedlist_t *mycharacter = NULL;
struct world_t *myworld = NULL;

void give_attribute(enum attribute_t attribute, int level) {
    mycharacter->TOP = node_append(mycharacter->TOP, new_listattribute(new_attributebonus(attribute, level)));
}

void give_skill(const char* name, enum attribute_t attribute, int level) {
    struct skill_t* sk = new_skill(name, attribute);
    myworld->skills = node_append(myworld->skills, sk);

    struct listitem_t *item = world_add_reference(myworld, name, level);
    mycharacter->TOP = node_append(mycharacter->TOP, (void*)item);
}

TEST_GROUP(character);

TEST_SETUP(character) {
    mycharacter = new_namedlist(list_rival, "Ralph");
    myworld = new_world();
    world_add_character(myworld, mycharacter);
}

TEST_TEAR_DOWN(character) {

}

TEST(character, single_attribute_values_create_correct_attribute_total) {
    struct listitem_t *attribute = new_listattribute(new_attributebonus(attr_cunning, 2));
    struct listitem_t *otherattribute = new_listattribute(new_attributebonus(attr_willpower, 1));
    struct namedlist_t *mychar = new_namedlist(list_rival, "Ralph");
    mychar->TOP = node_append(mychar->TOP, (void*)otherattribute);
    mychar->TOP = node_append(mychar->TOP, (void*)attribute);

    int actual = character_attribute(mychar, attr_cunning);
    TEST_ASSERT_EQUAL_INT(2, actual);
}

TEST(character, mismatched_attribute_values_return_zero) {
    struct listitem_t *attribute = new_listattribute(new_attributebonus(attr_presence, 1));
    struct namedlist_t *mychar = new_namedlist(list_rival, "Ralph");
    mychar->TOP = node_append(mychar->TOP, (void*)attribute);

    int actual = character_attribute(mychar, attr_cunning);
    TEST_ASSERT_EQUAL_INT(0, actual);
}

TEST(character, multiple_attribute_values_create_correct_attribute_total) {
    struct listitem_t *attribute = new_listattribute(new_attributebonus(attr_cunning, 2));
    struct listitem_t *attribute_redux = new_listattribute(new_attributebonus(attr_cunning, 1));
    struct listitem_t *otherattribute = new_listattribute(new_attributebonus(attr_willpower, 1));
    struct namedlist_t *mychar = new_namedlist(list_rival, "Ralph");
    mychar->TOP = node_append(mychar->TOP, (void*)attribute);
    mychar->TOP = node_append(mychar->TOP, (void*)otherattribute);
    mychar->TOP = node_append(mychar->TOP, (void*)attribute_redux);

    int actual = character_attribute(mychar, attr_cunning);
    TEST_ASSERT_EQUAL_INT(3, actual);
}

TEST(character, proficiency_when_proficiency_less_than_attribute_returns_proficiency) {
    give_attribute(attr_agility, 3);
    give_skill("Ranged-Light", attr_agility, 2);

    int actual = character_proficiency(mycharacter, "Ranged-Light");

    TEST_ASSERT_EQUAL_INT(2, actual);
}

TEST(character, proficiency_when_proficiency_greater_than_attribute_returns_attribute) {
    give_attribute(attr_agility, 3);
    give_skill("Ranged-Light", attr_agility, 4);

    int actual = character_proficiency(mycharacter, "Ranged-Light");

    TEST_ASSERT_EQUAL_INT(3, actual);
}

TEST(character, ability_when_attribute_greater_than_skill_level) {
    give_attribute(attr_cunning, 3);
    give_skill("Skulduggery", attr_cunning, 1);

    int actual = character_ability(myworld, mycharacter, "Skulduggery");

    TEST_ASSERT_EQUAL_INT(2, actual);
}

TEST(character, ability_when_attribute_less_than_skill_level) {
    give_attribute(attr_cunning, 2);
    give_skill("Skulduggery", attr_cunning, 3);

    int actual = character_ability(myworld, mycharacter, "Skulduggery");

    TEST_ASSERT_EQUAL_INT(1, actual);
}

TEST(character, ability_when_character_does_not_have_skill) {
    world_add_skill(myworld, new_skill("Skulduggery", attr_cunning));
    give_attribute(attr_cunning, 2);

    int actual = character_ability(myworld, mycharacter, "Skulduggery");

    TEST_ASSERT_EQUAL_INT(2, actual);
}

TEST(character, ability_when_skill_does_not_exist_is_zero) {

    int actual = character_ability(myworld, mycharacter, "Bogus Skill");

    TEST_ASSERT_EQUAL_INT(0, actual);
}

TEST_GROUP_RUNNER(character) {
    RUN_TEST_CASE(character, single_attribute_values_create_correct_attribute_total);
    RUN_TEST_CASE(character, mismatched_attribute_values_return_zero);
    RUN_TEST_CASE(character, multiple_attribute_values_create_correct_attribute_total);
    RUN_TEST_CASE(character, proficiency_when_proficiency_less_than_attribute_returns_proficiency);
    RUN_TEST_CASE(character, proficiency_when_proficiency_greater_than_attribute_returns_attribute);
    RUN_TEST_CASE(character, ability_when_attribute_greater_than_skill_level);
    RUN_TEST_CASE(character, ability_when_attribute_less_than_skill_level);
    RUN_TEST_CASE(character, ability_when_character_does_not_have_skill);
    RUN_TEST_CASE(character, ability_when_skill_does_not_exist_is_zero);
}
