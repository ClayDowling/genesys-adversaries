//
// Created by Clay on 1/20/2023.
//

#ifndef ADVERSARIES_WORLD_H
#define ADVERSARIES_WORLD_H

#include "ast.h"
#include "namedlist.h"

#include <map>
#include <string>

using std::map;
using std::string;

enum witype {skill, talent, package, character, weapon, armor};

struct world_item {
    world_item();

    enum witype type;
    union {
        skill_t skill;
        talent_t talent;
        const namedlist_t *list;
        weapon_t weapon;
        armor_t armor;
    };
};

extern struct world_item* EMPTY_ITEM;

class world{

private:
    map<string, world_item*> item;

    world_item* add_item(string name, witype type);

public:
    /// Returns EMPTY_ITEM is the named item does not exist in the world.
    struct world_item* get_item(string name);
    void add_skill(string name, enum attribute_t attribute);
    void add_talent(string name);
    void add_namedlist(string name, const namedlist_t*);
    void add_weapon(string name, string skill, bool brawl, int damage, int crit);
    void add_armor(string name, int defense, int soak);
};

#endif //ADVERSARIES_WORLD_H
