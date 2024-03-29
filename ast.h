#ifndef AST_H
#define AST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "token.h"

struct skill_t {
    const char *name;
    enum attribute_t attribute;
};

struct skill_t *new_skill(const char *n, enum attribute_t a);

bool is_skill(const void *, const void *);

struct talent_t {
    const char *name;
};

struct talent_t *new_talent(const char *n);

bool is_talent(const void *, const void *);

struct attributebonus_t {
    enum attribute_t attribute;
    int level;
};

struct attributebonus_t *new_attributebonus(enum attribute_t a, int l);

void delete_attributebonus(struct attributebonus_t *bonus);

struct skill_reference_t {
    const struct skill_t *reference;
    int level;
};

struct skill_reference_t *new_skill_reference(const struct skill_t *, int);

bool is_skill_reference(const void *, const void *);

bool is_skill_reference_name(const void *, const void *);

struct talent_reference_t {
    const struct talent_t *reference;
    int level;
};

struct talent_reference_t *new_talent_reference(const struct talent_t *, int);

bool is_talent_reference(const void *item, const void *talent);

bool is_talent_reference_name(const void *item, const void *name);

struct armor_t {
    const char *name;
    int defense;
    int soak;
};

struct armor_t *new_armor(const char *name, int defense, int soak);

bool is_armor(const void *candidate, const void *name);

bool is_armor_reference_name(const void *candidate, const void *name);

enum listitemtype {
    li_attribute, li_skillref, li_talentref, li_weapon, li_armor
};

/// listitem_t is contained in a named list such as a character or package
struct listitem_t {
    enum listitemtype type;
    union {
        struct attributebonus_t *bonus;
        struct skill_reference_t *skill;
        struct talent_reference_t *talent;
        struct weapon_t *weapon;
        struct armor_t *armor;
    };
};

struct listitem_t *new_listattribute(struct attributebonus_t *a);

struct listitem_t *new_listskill(struct skill_reference_t *n);

struct listitem_t *new_listtalent(struct talent_reference_t *n);

struct listitem_t *new_listweapon(struct weapon_t *w);

struct listitem_t *new_listarmor(struct armor_t *a);

struct node_t {
    void *node;
    struct node_t *next;
};


struct node_t *new_node(void *);

struct node_t *node_append(struct node_t *t, void *n);

void *node_find(struct node_t *, const void *, bool (predicate)(const void *, const void *));

struct skill_reference_t *node_find_skill_reference(struct node_t *TOP, const char *name);

struct talent_reference_t *node_find_talent_reference(struct node_t *TOP, const char *name);

enum namedlist_type {
    list_package, list_minion, list_rival, list_nemesis, list_archetype, list_MAX
};

const char *namedlist_type_name(enum namedlist_type t);

/// Contains a package or a character
struct namedlist_t {
    enum namedlist_type type;
    const char *name;
    struct node_t *TOP; /// Holds listitem_t
};

struct namedlist_t *new_namedlist(enum namedlist_type, const char *);

bool is_namedlist(const void *, const void *);

struct leveleditem_t {
    char *name;
    int level;
};

struct leveleditem_t *new_leveleditem(const char *, int);

void delete_leveleditem(struct leveleditem_t *item);

enum namedlistitemtype {
    nli_leveledname, nli_attribute
};

/// namedlistitem_t is read from a file and converted to a listitem_t
struct namedlistitem_t {
    enum namedlistitemtype type;
    union {
        struct leveleditem_t *item;
        struct attributebonus_t *bonus;
    };
};

struct namedlistitem_t *new_namedlistitem_leveled(struct leveleditem_t *item);

struct namedlistitem_t *new_namedlistitem_attribute(struct attributebonus_t *bonus);

void delete_namedlistitem(struct namedlistitem_t *nli);

struct weapon_t {
    const char *name;
    const char *skill;
    bool brawl;
    int damage;
    int crit;
    struct node_t *specials; // Contains leveleditem_t
};

struct weapon_t *new_weapon(const char *name, const char *skill, bool brawl, int damage, int crit);

void weapon_add_special(struct weapon_t *w, const char *special);

bool is_weapon(const void *candidate, const void *name);

bool is_weapon_reference_name(const void *candidate, const void *name);

struct world_t {
    struct node_t *root;
    struct node_t *skills;
    struct node_t *talents;
    struct node_t *packages;
    struct node_t *characters;
    struct node_t *weapons;
    struct node_t *armors;
};

void * new_world();

void world_add_skill(void *w, const char *name, enum attribute_t attr);

void world_add_talent(void *w, const char *name);

void world_add_package(void *w, const struct namedlist_t *nl);

void world_add_character(void *w, const struct namedlist_t *ch);

void world_add_weapon(void *w, const char *name, const char *skillname, bool brawl, int dmg, int crit);

struct listitem_t *world_add_reference(void *w, const char *n, int lvl);

void namedlist_add_reference(struct world_t *w, struct namedlist_t *list, struct namedlistitem_t *nli);

struct skill_t *world_find_skill(void *w, const char *n);

struct talent_t *world_find_talent(void *w, const char *n);

const struct namedlist_t *world_find_package(void *w, const char *n);

const struct namedlist_t *world_find_character(void *w, const char *n);

struct weapon_t *world_find_weapon(void *w, const char *n);

const struct namedlist_t** world_get_characters(void *w);

#ifdef __cplusplus
}
#endif

#endif
