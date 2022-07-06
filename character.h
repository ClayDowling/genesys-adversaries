//
// Created by Clay on 7/1/2022.
//

#ifndef ADVERSARIES_CHARACTER_H
#define ADVERSARIES_CHARACTER_H

#include "ast.h"
#include <stdio.h>

int character_attribute(struct namedlist_t* c, enum attribute_t attribute);
int character_proficiency(struct namedlist_t *c, const char *skillname);
int character_ability(struct world_t *w, struct namedlist_t *c, const char *skillname);

void print_character_rival(FILE *out, struct world_t *w, struct namedlist_t *c);

#endif //ADVERSARIES_CHARACTER_H
