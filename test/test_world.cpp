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