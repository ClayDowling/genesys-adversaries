#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct skill_t *new_skill(const char *n, enum attribute_t a) {
  struct skill_t *s = calloc(1, sizeof(struct skill_t));
  s->name = n;
  s->attribute = a;
  return s;
}

#define IS_NAME(TYPE, NODE, NAME)                                              \
  {                                                                            \
    TYPE *t = (TYPE *)NODE;                                                    \
    if (strcasecmp(NAME, t->name) == 0) {                                      \
      return true;                                                             \
    }                                                                          \
    return false;                                                              \
  }

bool is_skill(const void *node, const void *name) {
  IS_NAME(struct skill_t, node, name);
}

struct talent_t *new_talent(const char *n) {
  struct talent_t *t = (struct talent_t *)calloc(1, sizeof(struct talent_t));
  t->name = n;
  return t;
}

bool is_talent(const void *node, const void *name) {
  IS_NAME(struct talent_t, node, name);
}

struct attributebonus_t *new_attributebonus(enum attribute_t a, int l) {
  struct attributebonus_t *ab =
      (struct attributebonus_t *)calloc(1, sizeof(struct attributebonus_t));
  ab->attribute = a;
  ab->level = l;
  return ab;
}
void delete_attributebonus(struct attributebonus_t *bonus) {
    free(bonus);
}

struct skill_reference_t *new_skill_reference(const struct skill_t *s,
                                              int level) {
  struct skill_reference_t *sr =
      (struct skill_reference_t *)calloc(1, sizeof(struct skill_reference_t));
  sr->level = level;
  sr->reference = s;
  return sr;
}

struct talent_reference_t *new_talent_reference(const struct talent_t *r,
                                                int level) {
  struct talent_reference_t *tr =
      (struct talent_reference_t *)calloc(1, sizeof(struct talent_reference_t));
  tr->reference = r;
  tr->level = level;
  return tr;
}

bool is_skill_reference(const void *item, const void *skillreference) {
  struct listitem_t* li = (struct listitem_t*)item;
  if (li->type != li_skillref) {
      return false;
  }
  return li->skill->reference == (struct skill_t*)skillreference;
}

bool is_skill_reference_name(const void *item, const void *name) {
    struct listitem_t* li = (struct listitem_t*)item;
    if (li->type != li_skillref) {
        return false;
    }
    return strcasecmp(li->skill->reference->name, (const char*)name) == 0;
}

bool is_talent_reference(const void *item, const void *ref) {
    struct listitem_t* li = (struct listitem_t*)item;
    if (li->type != li_talentref) {
        return false;
    }
    return li->talent->reference == (struct talent_t*)ref;
}

bool is_talent_reference_name(const void* item, const void* name) {
    struct listitem_t* li = (struct listitem_t*)item;
    if (li->type != li_talentref) {
        return false;
    }
    return strcasecmp(li->talent->reference->name, (const char*)name) == 0;
}

struct listitem_t *new_listattribute(struct attributebonus_t *a) {
  struct listitem_t *li =
      (struct listitem_t *)calloc(1, sizeof(struct listitem_t));
  li->type = li_attribute;
  li->bonus = a;
  return li;
}

struct listitem_t *new_listskill(struct skill_reference_t *n) {
  struct listitem_t *li =
      (struct listitem_t *)calloc(1, sizeof(struct listitem_t));
  li->type = li_skillref;
  li->skill = n;
  return li;
}

struct listitem_t *new_listtalent(struct talent_reference_t *n) {
  struct listitem_t *li =
      (struct listitem_t *)calloc(1, sizeof(struct listitem_t));
  li->type = li_talentref;
  li->talent = n;
  return li;
}

struct listitem_t *new_listweapon(struct weapon_t *w) {
    struct listitem_t *li = (struct listitem_t*)calloc(1, sizeof(struct listitem_t));
    li->type = li_weapon;
    li->weapon = w;
    return li;
}

struct node_t *new_node(void *value) {
  struct node_t *n = (struct node_t *)calloc(1, sizeof(struct node_t));
  n->node = value;
  return n;
}

struct node_t *node_append(struct node_t *t, void *n) {
  if (NULL == t) {
    t = new_node(n);
    return t;
  }
  struct node_t *cur = t;
  while (cur->next != NULL)
    cur = cur->next;
  cur->next = new_node(n);
  return t;
}

void *node_find(struct node_t *top, const void *name,
                bool(predicate)(const void *, const void *)) {
  for (struct node_t *cur = top; cur != NULL; cur = cur->next) {
    if (predicate(cur->node, name)) {
      return cur->node;
    }
  }
  return NULL;
}

struct skill_reference_t *node_find_skill_reference(struct node_t *TOP, const char *name) {
    struct listitem_t *item = node_find(TOP, (void*)name, is_skill_reference_name);
    if (!item) return NULL;
    if (item->type == li_skillref) {
        return item->skill;
    }
    return NULL;
}

struct talent_reference_t *node_find_talent_reference(struct node_t *TOP, const char *name) {
    struct listitem_t *item = node_find(TOP, (void*)name, is_talent_reference_name);
    if (!item) return NULL;
    if (item->type == li_talentref) {
        return item->talent;
    }
    return NULL;
}


struct weapon_t* new_weapon(const char* name, const char* skill, bool brawl, int damage, int crit) {
  struct weapon_t* w = (struct weapon_t*)calloc(1, sizeof(struct weapon_t));
  w->name = strdup(name);
  w->skill = strdup(skill);
  w->brawl = brawl;
  w->damage = damage;
  w->crit = crit;
  return w;
}

void weapon_add_special(struct weapon_t* w, const char* special) {
  w->specials = node_append(w->specials, (void*)special);
}

bool is_weapon(const void* candidate, const void* name) {
    IS_NAME(struct weapon_t, candidate, name);
}

bool is_weapon_reference_name(const void* candidate, const void* name) {
    struct listitem_t *item = (struct listitem_t*)candidate;
    if (item->type == li_weapon) {
        return strcasecmp(item->weapon->name, (const char*)name) == 0;
    }
    return false;
}

struct armor_t* new_armor(const char* name, int defense, int soak) {
  struct armor_t *a = (struct armor_t*)calloc(1, sizeof(struct armor_t));
  a->name = strdup(name);
  a->defense = defense;
  a->soak = soak;

  return a;
}

bool is_armor(const void* candidate, const void* name) {
  IS_NAME(struct armor_t, candidate, name);
}

struct namedlist_t *new_namedlist(enum namedlist_type t, const char *n) {
  struct namedlist_t *nl =
      (struct namedlist_t *)calloc(1, sizeof(struct namedlist_t));
  nl->name = n;
  nl->type = t;
  return nl;
}

bool is_namedlist(const void *candidate, const void *name) {
  IS_NAME(struct namedlist_t, candidate, name);
}

struct leveleditem_t *new_leveleditem(const char *n, int l) {
  struct leveleditem_t *li =
      (struct leveleditem_t *)calloc(1, sizeof(struct leveleditem_t));
  li->name = strdup(n);
  li->level = l;
  return li;
}

void delete_leveleditem(struct leveleditem_t *item) {
    if (item->name) {
        free((void*)item->name);
        item->name = NULL;
    }
    free(item);
}

struct namedlistitem_t* new_namedlistitem_leveled(struct leveleditem_t* item) {
    struct namedlistitem_t *nli = (struct namedlistitem_t*)calloc(1, sizeof(struct namedlistitem_t));
    nli->type = nli_leveledname;
    nli->item = item;
    return nli;
}

struct namedlistitem_t* new_namedlistitem_attribute(struct attributebonus_t* bonus) {
    struct namedlistitem_t *nli = (struct namedlistitem_t*)calloc(1, sizeof(struct namedlistitem_t));
    nli->type = nli_attribute;
    nli->bonus = bonus;
    return nli;
}

void delete_namedlistitem(struct namedlistitem_t* nli) {
    switch(nli->type) {
        case nli_attribute:
            delete_attributebonus(nli->bonus);
            break;
        case nli_leveledname:
            delete_leveleditem(nli->item);
            break;
    }
}

void namedlist_add_reference(struct world_t* w, struct namedlist_t* list, struct namedlistitem_t* nli) {
    if (NULL == w) return;
    if (NULL == list) return;
    if (NULL == nli) return;



    if (nli_attribute == nli->type) {
        struct listitem_t *item = new_listattribute(nli->bonus);
        list->TOP = node_append(list->TOP, (void*)item);
    }

    if (nli_leveledname == nli->type) {

        const struct namedlist_t *pkg = world_find_package(w, nli->item->name);
        if (NULL != pkg) {
          for(struct node_t *cur=pkg->TOP; NULL != cur; cur = cur->next) {
            list->TOP = node_append(list->TOP, cur->node);
          }
        }
        else {
          struct listitem_t *item = world_add_reference(w, nli->item->name, nli->item->level);
          if (item) {
              list->TOP = node_append(list->TOP, item);
          } else {
              fprintf(stderr, "Item \"%s\" not found in world.\n", nli->item->name);
          }
        }
    }
}

const char* namedlist_type_name(enum namedlist_type t) {
    switch(t) {
        case list_package:
            return "package";
        case list_minion:
            return "minion";
        case list_rival:
            return "rival";
        case list_nemesis:
            return "nemesis";
        case list_MAX:
            return "MAX";
        default:
            return "UNKNOWN";
    }
}

