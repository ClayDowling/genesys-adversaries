//
// Created by Clay on 7/1/2022.
//

#include <stdio.h>
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

void print_talent(FILE* out, struct world_t* w, struct namedlist_t* c, struct listitem_t *item) {
    const char* name = item->talent->reference->name;
    if (item->talent->level == 0) {
        fprintf(out, "%s", name);
    } else {
        fprintf(out, "%s %d", name, item->talent->level);
    }
}

void print_skill(FILE* out, struct world_t *w, struct namedlist_t *c, struct listitem_t *item) {
    const char* name = item->skill->reference->name;
    fprintf(out, "%s ", name);
    int proficiency = character_proficiency(c, name);
    int ability = character_ability(w, c, name);
    for(int i=0; i < proficiency; ++i) fputc('Y', out);
    for(int i=0; i < ability; ++i) fputc('g', out);
}

void print_list(FILE *out, struct world_t *w, struct namedlist_t *c,
        enum listitemtype type, void (*printfunction)(FILE*, struct world_t*, struct namedlist_t*, struct listitem_t*)) {
    const char *NOCOMMA = "";
    const char *COMMA = ", ";
    struct listitem_t *item;
    const char *sep = NOCOMMA;

    for(struct node_t *cur = c->TOP; cur != NULL; cur = cur->next) {
        item = (struct listitem_t*)cur->node;
        if (item->type == type) {
            fprintf(out, sep);
            sep = COMMA;
            printfunction(out, w, c, item);
        }
    }
    if (sep == COMMA) fprintf(out, "\n");
}

void print_character_rival(FILE *out, struct world_t *w, struct namedlist_t *c) {
    fprintf(out, "Br Ag Int Cun Will Pres\n"
                 "-- -- --- --- ---- ----\n"
                 "%2d %2d %3d %3d %4d %4d\n\n",
            character_attribute(c, attr_brawn),
            character_attribute(c, attr_agility),
            character_attribute(c, attr_intellect),
            character_attribute(c, attr_cunning),
            character_attribute(c, attr_willpower),
            character_attribute(c, attr_presence)
    );

    print_list(out, w, c, li_talentref, print_talent);
    print_list(out, w, c, li_skillref, print_skill);

}

