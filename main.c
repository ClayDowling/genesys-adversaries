#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "character.h"

void about(void) {
    puts("Genesys RPG Adversary Builder\n"
         "\n"
         "Copyright (c) 2022 by Clay Dowling\n"
         "All rights reserved.\n"
    );
}

int main(int argc, const char* argv[]) {

    if (argc == 1) {
        about();
        return EXIT_FAILURE;
    }

    struct world_t *world = parse_file(argv[1]);
    struct namedlist_t *character;
    int count = 0;
    for(struct node_t *cur = world->characters; cur != NULL; cur = cur->next) {
        count++;
        character = (struct namedlist_t*)cur->node;
        print_character_rival(stdout, world, character);
    }
    printf("%d characters total\n", count);

    return EXIT_SUCCESS;
}
