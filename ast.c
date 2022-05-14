#include "ast.h"
#include <stdlib.h>
#include <strings.h>

struct skill_t* new_skill(const char* n, enum attribute_t a) {
    struct skill_t *s = calloc(1, sizeof(struct skill_t));
    s->name = n;
    s->attribute = a;
    return s;
}

#define IS_NAME(TYPE, NODE, NAME)           \
{ TYPE* t = (TYPE*) NODE ;                  \
    if (strcasecmp(NAME, t->name) == 0) {   \
        return true;                        \
    }                                       \
    return false;                           \
}

bool is_skill(const void *node, const char* name) {
    IS_NAME(struct skill_t, node, name);
}

struct talent_t* new_talent(const char* n) {
    struct talent_t* t = (struct talent_t*)calloc(1, sizeof(struct talent_t));
    t->name = n;
    return t;
}

bool is_talent(const void* node, const char* name) {
    IS_NAME(struct talent_t, node, name);
}

struct attributebonus_t* new_attributebonus(enum attribute_t a, int l) {
    struct attributebonus_t* ab = (struct attributebonus_t*)calloc(1, sizeof(struct attributebonus_t));
    ab->attribute = a;
    ab->level = l;
    return ab;
}

#define IS_REFERENCE(TYPE, NODE, POINTER) {             \
    if (NODE->reference == (TYPE*)POINTER) return true; \
    return false;                                       \
}

struct listitem_t* new_listattribute(const struct attributebonus_t* a) {
    struct listitem_t* li = (struct listitem_t*)calloc(1, sizeof(struct listitem_t));
    li->type = li_attribute;
    li->bonus = a;
    return li;
}

struct listitem_t* new_listskill(const struct skill_reference_t* n) {
    struct listitem_t* li = (struct listitem_t*)calloc(1, sizeof(struct listitem_t));
    li->type = li_skillref;
    li->skill = n;
    return li;
}

struct listitem_t* new_listtalent(const struct talent_reference_t* n) {
    struct listitem_t* li = (struct listitem_t*)calloc(1, sizeof(struct listitem_t));
    li->type = li_talentref;
    li->talent = n;
    return li;
}

struct node_t* new_node(void* value) {
    struct node_t* n = (struct node_t*)calloc(1, sizeof(struct node_t));
    n->node = value;
    return n;
}

struct node_t* node_append(struct node_t* t, void* n) {
    if (NULL == t) {
        t = new_node(n);
        return t;
    }
    struct node_t* cur = t;
    while(cur->next != NULL) cur = cur->next;
    cur->next = new_node(n);
    return t;
}

void* node_find(struct node_t* top, const char* name, bool (predicate)(const void*, const char*)) {
    for(struct node_t* cur = top; cur != NULL; cur = cur->next) {
        if (predicate(cur->node, name)) {
            return cur->node;
        }
    }
    return NULL;
}

struct namedlist_t* new_namedlist(enum namedlist_type t, const char* n) {
    struct namedlist_t* nl = (struct namedlist_t*)calloc(1, sizeof(struct namedlist_t));
    nl->name = n;
    nl->type = t;
    return nl;
}

bool is_namedlist(const void* candidate, const char* name) {
    IS_NAME(struct namedlist_t, candidate, name);
}

struct world_t* new_world() {
    struct world_t* w = (struct world_t*)calloc(1, sizeof(struct world_t));
    return w;
}

void world_add_skill(struct world_t* w, const struct skill_t* s) {
    w->skills = node_append(w->skills, (void*)s);
}

void world_add_talent(struct world_t* w, const struct talent_t* t) {
    w->talents = node_append(w->talents, (void*)t);
}

void world_add_package(struct world_t* w, const struct namedlist_t* p) {
    w->packages = node_append(w->packages, (void*)p);
}

void world_add_character(struct world_t* w, const struct namedlist_t* c) {
    w->characters = node_append(w->characters, (void*)c);
}

struct skill_t* world_find_skill(struct world_t* w, const char* n) { 
    return (struct skill_t*)node_find(w->skills, n, is_skill);
}

struct talent_t* world_find_talent(struct world_t* w, const char* n) {
    return (struct talent_t*)node_find(w->talents, n, is_talent);
}

struct namedlist_t* world_find_package(struct world_t* w, const char* n) {
    return (struct namedlist_t*)node_find(w->packages, n, is_namedlist);
}

struct namedlist_t* world_find_character(struct world_t* w, const char* n) {
    return (struct namedlist_t*)node_find(w->characters, n, is_namedlist);
}