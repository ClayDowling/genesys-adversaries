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

struct leveledname_t {
    const char* name;
    int level;
};
struct leveledname_t* new_leveledname(const char* n, int l);

enum listitemtype { li_attribute, li_name };

struct listitem_t {
    enum listitemtype type;
    union {
        const struct attributebonus_t* bonus;
        const struct leveledname_t* item;
    };
};
struct listitem_t* new_listattribute(enum listitemtype t, const struct attributebonus_t* a);
struct listitem_t* new_listname(enum listitemtype t, const struct leveledname_t* n);

struct node_t {
    void* node;
    struct node_t *next;
};
struct node_t* new_node(void*);
struct node_t* node_append(struct node_t* t, void* n);
void* node_find(struct node_t*, const char*, bool (predicate)(const void*, const char*));

struct namedlist_t {
    const char *name;
    struct node_t TOP;
};
struct namedlist_t* new_namedlist(const char* n);

struct world_t {
    struct node_t *skills;
    struct node_t *talents;
    struct node_t *packages;
    struct node_t *characters;
};
struct world_t* new_world();
struct skill_t* find_skill(struct world_t* w, const char* n);
struct talent_t* find_talent(struct world_t* w, const char* n);
struct package_t* find_package(struct world_t* w, const char* n);

#endif