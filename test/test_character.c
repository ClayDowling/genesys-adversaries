//
// Created by Clay on 7/2/2022.
//

#include <unity_fixture.h>
#include "ast.h"
#include "character.h"

TEST_GROUP(character);

TEST_SETUP(character) {

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


TEST_GROUP_RUNNER(character) {
    RUN_TEST_CASE(character, single_attribute_values_create_correct_attribute_total);
    RUN_TEST_CASE(character, mismatched_attribute_values_return_zero);
}
