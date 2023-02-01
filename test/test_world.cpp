//
// Created by Clay on 1/30/2023.
//

#include <catch2/catch_test_macros.hpp>
#include "world.h"

#include <cstring>

const char *WORLD_TAG = "[world]";

TEST_CASE("world_find_package returns package when present", WORLD_TAG) {
    world w;
    namedlist_t *pkg = new_namedlist(namedlist_type::list_package, "demo");
    w.add_namedlist("demo", pkg);

    auto item = world_find_package(&w, "demo");
    REQUIRE( item != nullptr );
    REQUIRE( strcmp(item->name, "demo") == 0 );
    REQUIRE( item->type == namedlist_type::list_package );
}

TEST_CASE("world_find_package returns EMPTY_ITEM when package is not present", WORLD_TAG) {
    world w;
    namedlist_t *pkg = new_namedlist(namedlist_type::list_package, "demo");
    w.add_namedlist("demo", pkg);

    auto item = world_find_package(&w, "production");

    REQUIRE( item == nullptr );
}

TEST_CASE("world_find_package returns nullptr when item is found but is not a package", WORLD_TAG) {
    const char* NAME = "Sneaking";
    world w;
    w.add_talent(NAME);

    auto item = world_find_package(&w, NAME);

    REQUIRE( item == nullptr );
}

TEST_CASE("world_find_character returns character when present in the world", WORLD_TAG) {
    world w;
    const char *NAME = "Franky";
    struct namedlist_t *ch = new_namedlist(namedlist_type::list_rival, NAME);
    w.add_namedlist(NAME, ch);

    auto item = world_find_character(&w, NAME);

    REQUIRE( item != nullptr );
    REQUIRE( item->type == namedlist_type::list_rival );
    REQUIRE( strcmp(item->name, NAME) == 0 );
}

TEST_CASE("world_find_character returns nullptr when item with the name is found but is not a character", WORLD_TAG) {
    world w;
    const char *NAME = "Franky";
    w.add_skill(NAME, attr_agility);

    auto item = world_find_character(&w, NAME);

    REQUIRE( item == nullptr );
}

TEST_CASE("world_find_skill returns skill when item with the name is found", WORLD_TAG) {
    world w;
    const char *NAME = "Stealing";
    w.add_skill(NAME, attr_agility);

    auto item = world_find_skill(&w, NAME);

    REQUIRE( item != nullptr );
    REQUIRE( strcmp(item->name, NAME) == 0 );
    REQUIRE( item->attribute == attr_agility );
}

TEST_CASE("world_find_skill returns nullptr when item with the name is found but is not skill", WORLD_TAG) {
    world w;
    const char *NAME = "Stealing";
    w.add_talent(NAME);

    auto item = world_find_skill(&w, NAME);

    REQUIRE( item == nullptr );
}

TEST_CASE("world_find_talent returns talent when it is in the world", WORLD_TAG) {
    world w;
    const char *NAME = "Dirty Pool";
    w.add_talent(NAME);

    auto item = world_find_talent(&w, NAME);

    REQUIRE( item != nullptr );
    REQUIRE( strcmp(item->name, NAME) == 0 );
}

TEST_CASE("world_find_talent returns nullptr when it is found but isn't a skill", WORLD_TAG) {
    world w;
    const char *NAME = "Dirty Pool";
    w.add_skill(NAME, attr_intellect);

    auto item = world_find_talent(&w, NAME);

    REQUIRE( item == nullptr );
}

TEST_CASE("world_find_weapon returns weapon when it exists in the world", WORLD_TAG) {
    world w;
    const char *NAME = "Shiv";
    w.add_weapon(NAME, "Stabbing", false, 3, 4);

    auto item = world_find_weapon(&w, NAME);

    REQUIRE( item != nullptr );
    REQUIRE( strcmp(item->name, NAME) == 0 );
    REQUIRE( strcmp(item->skill, "Stabbing") == 0);
    REQUIRE( item->brawl == false );
    REQUIRE( item->damage == 3 );
    REQUIRE( item->crit == 4 );
}

TEST_CASE("world_find_weapon returns nullptr when it exists but isn't a weapon", WORLD_TAG) {
    world w;
    const char *NAME = "Shiv";
    w.add_talent(NAME);

    auto item = world_find_weapon(&w, NAME);

    REQUIRE( item == nullptr );
}

TEST_CASE("get_characters returns one character when there is a character in the world", WORLD_TAG) {
    world w;
    const char *NAME = "Sam";
    namedlist_t *sam = new_namedlist(namedlist_type::list_rival, NAME);
    w.add_namedlist(NAME, sam);

    auto actual = w.get_characters();

    REQUIRE( actual != nullptr );
    REQUIRE( actual[0] == sam );
    REQUIRE( actual[1] == nullptr );
}