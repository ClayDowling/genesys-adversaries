#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include "token.h"

struct skill_t {
    const char* name;
    enum attribute_t attribute;
};

struct skill_t* new_skill(const char* n, enum attribute_t a);
bool is_skill(const void*, const char*);

struct talent_t {
    const char* name;
};

struct talent_t* new_talent(const char* n);
bool is_talent(const void*, const char*);

struct attributebonus_t {
    enum attribute_t attribute;
    int level;
};
struct attributebonus_t* new_attributebonus(enum attribute_t a, int l);

struct skill_reference_t {
    const struct skill_t* reference;
    int level;
};
struct skill_reference_t* new_skill_reference(const struct skill_t*, int);
bool is_skill_reference(const void*, struct skill_t*);

struct talent_reference_t {
    const struct talent_t* reference;
    int level;
};
struct talent_reference_t* new_talent_reference(const struct talent_t*, int);
bool is_talent_reference(const void*, struct talent_t*);

enum listitemtype { li_attribute, li_skillref, li_talentref };

struct listitem_t {
    enum listitemtype type;
    union {
        const struct attributebonus_t* bonus;
        const struct skill_reference_t* skill;
        const struct talent_reference_t* talent;
    };
};
struct listitem_t* new_listattribute(const struct attributebonus_t* a);
struct listitem_t* new_listskill(const struct skill_reference_t* n);
struct listitem_t* new_listtalent(const struct talent_reference_t* n);

struct node_t {
    void* node;
    struct node_t *next;
};
struct node_t* new_node(void*);
struct node_t* node_append(struct node_t* t, void* n);
void* node_find(struct node_t*, const char*, bool (predicate)(const void*, const char*));

enum namedlist_type {list_package, list_minion, list_rival, list_nemesis, list_MAX};

struct namedlist_t {
    enum namedlist_type type;
    const char* name;
    struct node_t* TOP;
};
struct namedlist_t* new_namedlist(enum namedlist_type, const char*);
bool is_namedlist(const void*, const char*);

struct leveleditem_t {
	const char* name;
	int level;
};
struct leveleditem_t* new_leveleditem(const char*, int);

struct world_t {
    struct node_t *skills;
    struct node_t *talents;
    struct node_t *packages;
    struct node_t *characters;
};
struct world_t* new_world();

void world_add_skill(struct world_t*, const struct skill_t*);
void world_add_talent(struct world_t*, const struct talent_t*);
void world_add_package(struct world_t*, const struct namedlist_t*);
void world_add_character(struct world_t*, const struct namedlist_t*);

struct listitem_t* world_add_reference(const struct world_t*, const char*, int);

struct skill_t* world_find_skill(const struct world_t* w, const char* n);
struct talent_t* world_find_talent(const struct world_t* w, const char* n);
struct namedlist_t* world_find_package(const struct world_t* w, const char* n);
struct namedlist_t* world_find_character(const struct world_t*, const char*);

#endif
