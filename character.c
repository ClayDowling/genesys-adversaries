//
// Created by Clay on 7/1/2022.
//

#include <stdlib.h>
#include "character.h"

int character_attribute(struct namedlist_t* c, enum attribute_t attribute) {
    struct listitem_t *item;
    for(struct node_t *cur = c->TOP; cur != NULL; cur = cur->next) {
        item = (struct listitem_t*)cur->node;
        if (item->type == li_attribute) {
            struct attributebonus_t *ab = item->bonus;
            if (ab->attribute == attribute) return ab->level;
        }
    }
    return 0;
}
