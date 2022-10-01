//
// Created by Clay on 7/1/2022.
//

#ifndef ADVERSARIES_CHARACTER_H
#define ADVERSARIES_CHARACTER_H

#include "ast.h"
#include "cJSON.h"
#include <stdio.h>

int character_attribute(struct namedlist_t* c, enum attribute_t attribute);

/** character_wound calculates the wound threshold for the character.  Technically an attribute, but the final
 * value of wound is the combination of wound and brawn.
 *
 * @param c namedlist_t representing the character
 * @return int wound threshold
 */
int character_wound(struct namedlist_t* c);

/** character_strain calculcate the strain threshold for the character.  Technically an attribute, but the final
 *  value of strain is the combination of strain and will.
 *
 * @param c namedlist_t representing the character
 * @return int strain threshold
 */
int character_strain(struct namedlist_t* c);

int character_proficiency(struct namedlist_t *c, const char *skillname);
int character_ability(struct world_t *w, struct namedlist_t *c, const char *skillname);

void print_character_rival(FILE *out, struct world_t *w, struct namedlist_t *c);

struct cJSON* character_json(struct world_t *w, struct namedlist_t *c);
void print_character_json(FILE *out, struct world_t *w, struct namedlist_t *c);

#endif //ADVERSARIES_CHARACTER_H
