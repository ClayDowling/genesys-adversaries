//
// Created by Clay on 7/1/2022.
//

#include <stdlib.h>
#include <string.h>
#include "character.h"

int character_attribute(struct namedlist_t* c, enum attribute_t attribute) {
    struct listitem_t *item;
    int total = 0;
    for(struct node_t *cur = c->TOP; cur != NULL; cur = cur->next) {
        item = (struct listitem_t*)cur->node;
        if (item->type == li_attribute) {
            struct attributebonus_t *ab = item->bonus;
            if (ab->attribute == attribute) total += ab->level;
        }
    }
    return total;
}

int character_proficiency(struct namedlist_t *c, const char *skillname) {
    struct skill_reference_t *ref = node_find_skill_reference(c->TOP, skillname);
    if (ref) {
        int attribute = character_attribute(c, ref->reference->attribute);
        return attribute > ref->level ? ref->level : attribute;
    }
    return 0;
}

int character_ability(struct world_t *w, struct namedlist_t *c, const char *skillname) {
    struct skill_reference_t *ref = node_find_skill_reference(c->TOP, skillname);
    if (ref) {
        int attribute = character_attribute(c, ref->reference->attribute);
        if (attribute > ref->level) return attribute - ref->level;
        return ref->level - attribute;
    } else {
        struct skill_t *skill = world_find_skill(w, skillname);
        if (skill) {
            return character_attribute(c, skill->attribute);
        }
    }
    return 0;
}

