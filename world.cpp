//
// Created by Clay on 1/20/2023.
//

#include "world.h"

#include <cstring>

using ::strdup;

struct world_item *EMPTY_ITEM = nullptr;

struct world_item* world::get_item(string name) {
    if (item.find(name) == item.end())
        return EMPTY_ITEM;
    return item[name];
}

void world::add_skill(string name, enum attribute_t attribute) {
    auto s = add_item(name, witype::skill);
    s->skill.attribute = attribute;
    s->skill.name = strdup(name.c_str());
}

void world::add_talent(string name) {
    auto s = add_item(name, witype::talent);
    s->talent.name = strdup(name.c_str());
}

void world::add_namedlist(string name, const namedlist_t *lst) {
    witype listtype;
    switch(lst->type) {
        case namedlist_type::list_minion:
        case namedlist_type::list_rival:
        case namedlist_type::list_nemesis:
            listtype = witype::character;
            break;
        case namedlist_type::list_archetype:
        case namedlist_type::list_package:
        default:
            listtype = witype::package;
    }
    auto nl = add_item(name, listtype);
    nl->list = lst;
}

void world::add_weapon(string name, string skill, bool brawl, int damage, int crit) {
    auto w = add_item(name, witype::weapon);
    w->weapon.name = strdup(name.c_str());
    w->weapon.skill = strdup(skill.c_str());
    w->weapon.brawl = brawl;
    w->weapon.crit = crit;
    w->weapon.damage = damage;
    w->weapon.specials = nullptr;
}

void world::add_armor(string name, int defense, int soak) {
    auto a = add_item(name, witype::armor);
    a->armor.name = strdup(name.c_str());
    a->armor.defense = defense;
    a->armor.soak = soak;
}

world_item *world::add_item(string name, witype type) {
    world_item *i = new world_item();
    i->type = type;
    item[name] = i;
    return i;
}

world_item::world_item() {

}

void *new_world() {
    return (void*) new world();
}

world *getworld(void* w) {
    return (world*)w;
}

void world_add_skill(void *w, const char *s, enum attribute_t attr) {
    auto wrld = getworld(w);
    wrld->add_skill(s, attr);
}

void world_add_talent(void *w, const char *t) {
    auto wrld = getworld(w);
    wrld->add_talent(t);
}

void world_add_package(void *w, const struct namedlist_t *p) {
    auto wrld = getworld(w);
    wrld->add_namedlist(p->name, p);
}

void world_add_character(void *w, const struct namedlist_t *c) {
    auto wrld = getworld(w);
    wrld->add_namedlist(c->name, c);
}

void world_add_weapon(void *w, const char *name, const char *skillname, bool brawl, int dmg, int crit) {
    auto wrld = getworld(w);
    wrld->add_weapon(name, skillname, brawl, dmg, crit);
}


struct skill_t *world_find_skill(void *w, const char *n) {
    auto item = getworld(w)->get_item(n);
    return ( item && item->type == witype::skill ) ? &item->skill : nullptr;
}

struct talent_t *world_find_talent(void *w, const char *n) {
    auto item = getworld(w)->get_item(n);
    return ( item && item->type == witype::talent ) ? &item->talent : nullptr;
}

const struct namedlist_t *world_find_package(void *w, const char *n) {
    auto item = getworld(w)->get_item(n);
    return (item && item->type == witype::package) ? item->list : nullptr;
}

const struct namedlist_t *world_find_character(void *w, const char *n) {
    auto item = getworld(w)->get_item(n);
    return (item && item->type == witype::character ) ? item->list : nullptr;
}

struct weapon_t *world_find_weapon(void *w, const char *n) {
    auto item = getworld(w)->get_item(n);
    return ( item && item->type == witype::weapon) ? &item->weapon : nullptr;
}

struct listitem_t *world_add_reference(void *w, const char *n, int lvl) {
    struct listitem_t *li = NULL;

    auto item = getworld(w)->get_item(n);
    if (item == EMPTY_ITEM) return nullptr;

    switch(item->type) {
        case witype::skill:
        if (lvl == 0) lvl = 1;
        li = new_listskill(new_skill_reference(&item->skill, lvl));
        break;
        case witype::talent:
        li = new_listtalent(new_talent_reference(&item->talent, lvl));
        break;
        case witype::weapon:
        li = new_listweapon(&item->weapon);
        break;
    }

    return li;
}

