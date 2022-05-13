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

bool is_skill(void *node, const char* name) {
    IS_NAME(struct skill_t, node, name);
}

struct talent_t* new_talent(const char* n) {
    struct talent_t* t = (struct talent_t*)calloc(1, sizeof(struct talent_t));
    t->name = n;
    return t;
}

bool is_talent(void* node, const char* name) {
    IS_NAME(struct talent_t, node, name);
}

struct attributebonus_t* new_attributebonus(enum attribute_t a, int l) {
    struct attributebonus_t* ab = (struct attributebonus_t*)calloc(1, sizeof(struct attributebonus_t));
    ab->attribute = a;
    ab->level = l;
    return ab;
}

struct leveledname_t* new_leveledname(const char* n, int l) {
    struct leveledname_t* ln = (struct leveledname_t*)calloc(1, sizeof(struct leveledname_t));
    ln->name = n;
    ln->level = l;
    return ln;
}

struct listitem_t* new_listattribute(enum listitemtype t, const struct attributebonus_t* a) {
    struct listitem_t* li = (struct listitem_t*)calloc(1, sizeof(struct listitem_t));
    li->type = li_attribute;
    li->bonus = a;
    return li;
}

struct listitem_t* new_listname(enum listitemtype t, const struct leveledname_t* n) {
    struct listitem_t* li = (struct listitem_t*)calloc(1, sizeof(struct listitem_t));
    li->type = li_name;
    li->item = n;
    return li;
}

struct node_t* new_node(void* value) {
    struct node_t* n = (struct node_t*)calloc(1, sizeof(struct node_t));
    n->node = value;
    return n;
}

void node_append(struct node_t* t, void* n) {
    if (NULL == t) {
        t = new_node(n);
        return t;
    }
    struct node_t* cur = t;
    while(cur->next != NULL) cur = cur->next;
    cur->next = new_node(n);
    return t;
}

struct namedlist_t* newnamedlist(const char* n) {
    struct namedlist_t* nl = (struct namedlist_t*)calloc(1, sizeof(struct namedlist_t));
    nl->name = n;
    return nl;
}