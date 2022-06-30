#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include "token.h"

struct skill_t {
    const char* name;
    enum attribute_t attribute;
};

struct skill_t* new_skill(const char* n, enum attribute_t a);
bool is_skill(const void*, const void*);

struct talent_t {
    const char* name;
};

struct talent_t* new_talent(const char* n);
bool is_talent(const void*, const void*);

struct attributebonus_t {
    enum attribute_t attribute;
    int level;
};
struct attributebonus_t* new_attributebonus(enum attribute_t a, int l);
void delete_attributebonus(struct attributebonus_t *bonus);

struct skill_reference_t {
    const struct skill_t* reference;
    int level;
};
struct skill_reference_t* new_skill_reference(const struct skill_t*, int);
bool is_skill_reference(const void*, const void*);
bool is_skill_reference_name(const void*, const void*);

struct talent_reference_t {
    const struct talent_t* reference;
    int level;
};
struct talent_reference_t* new_talent_reference(const struct talent_t*, int);
bool is_talent_reference(const void* item, const void* talent);
bool is_talent_reference_name(const void* item, const void* name);

enum listitemtype { li_attribute, li_skillref, li_talentref, li_weapon };

struct listitem_t {
    enum listitemtype type;
    union {
        const struct attributebonus_t* bonus;
        const struct skill_reference_t* skill;
        const struct talent_reference_t* talent;
        const struct weapon_t* weapon;
    };
};
struct listitem_t* new_listattribute(const struct attributebonus_t* a);
struct listitem_t* new_listskill(const struct skill_reference_t* n);
struct listitem_t* new_listtalent(const struct talent_reference_t* n);
struct listitem_t *new_listweapon(const struct weapon_t *w);

struct node_t {
    void* node;
    struct node_t *next;
};
struct node_t* new_node(void*);
struct node_t* node_append(struct node_t* t, void* n);
void* node_find(struct node_t*, const void*, bool (predicate)(const void*, const void*));

enum namedlist_type {list_package, list_minion, list_rival, list_nemesis, list_MAX};

struct namedlist_t {
    enum namedlist_type type;
    const char* name;
    struct node_t* TOP;
};
struct namedlist_t* new_namedlist(enum namedlist_type, const char*);
bool is_namedlist(const void*, const void*);

struct leveleditem_t {
	char* name;
	int level;
};
struct leveleditem_t* new_leveleditem(const char*, int);
void delete_leveleditem(struct leveleditem_t *item);

enum namedlistitemtype { nli_leveledname, nli_attribute };

struct namedlistitem_t {
    enum namedlistitemtype type;
    union {
        struct leveleditem_t* item;
        struct attributebonus_t* bonus;
    };
};
struct namedlistitem_t* new_namedlistitem_leveled(struct leveleditem_t* item);
struct namedlistitem_t* new_namedlistitem_attribute(struct attributebonus_t* bonus);
void delete_namedlistitem(struct namedlistitem_t* nli);

struct weapon_t {
    char *name;
    char *skill;
    bool brawl;
    int damage;
    int crit;
    struct node_t* specials;
};

struct weapon_t* new_weapon(const char* name, const char* skill, bool brawl, int damage, int crit);
void weapon_add_special(struct weapon_t* w, const char* special);
bool is_weapon(const void* candidate, const void* name);
bool is_weapon_reference_name(const void* candidate, const void* name);

struct world_t {
    struct node_t *skills;
    struct node_t *talents;
    struct node_t *packages;
    struct node_t *characters;
    struct node_t *weapons;
};
struct world_t* new_world();

void world_add_skill(struct world_t*, const struct skill_t*);
void world_add_talent(struct world_t*, const struct talent_t*);
void world_add_package(struct world_t*, const struct namedlist_t*);
void world_add_character(struct world_t*, const struct namedlist_t*);
void world_add_weapon(struct world_t*, const struct weapon_t*);

struct listitem_t* world_add_reference(const struct world_t*, const char*, int);
void namedlist_add_reference(const struct world_t* w, struct namedlist_t* list, struct namedlistitem_t* nli);

struct skill_t* world_find_skill(const struct world_t* w, const char* n);
struct talent_t* world_find_talent(const struct world_t* w, const char* n);
struct namedlist_t* world_find_package(const struct world_t* w, const char* n);
struct namedlist_t* world_find_character(const struct world_t*, const char*);
struct weapon_t* world_find_weapon(const struct world_t*, const char*);

#endif
