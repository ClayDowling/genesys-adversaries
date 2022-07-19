#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "configure.h"
#include "parser.h"
#include "character.h"
#include "lexer.h"

void about(void) {
    printf("Genesys RPG Adversary Builder v%s\n"
         "\n"
         "Copyright (c) 2022 by Clay Dowling\n"
         "All rights reserved.\n", ADVERSARY_VERSION
    );
}

const char* basename(const char* filename) {
    static char path[PATH_MAX];
    char *end;
#ifdef _WIN64
    end = strrchr(filename, '\\');
#else
    end = strrchr(filename, '/');
#endif
    if (NULL == end) return ".";
    strncpy(path, filename, end - filename);
    return path;
}

int main(int argc, const char* argv[]) {

    if (argc == 1) {
        about();
        return EXIT_FAILURE;
    }

    char librarypath[PATH_MAX];
#ifdef _WIN64
    snprintf(librarypath, PATH_MAX, "%s\\data", basename(argv[0]));
#else
    snprintf(librarypath, PATH_MAX, "%s/data", basename(argv[0]));
#endif
    lex_add_directory(librarypath);

    const char* libraryenv = getenv("ADVERSARIES_LIBRARY");
    if (libraryenv) {
        lex_add_directory(libraryenv);
    }

    struct world_t *world = parse_file(argv[1]);
    struct namedlist_t *character;
    for(struct node_t *cur = world->characters; cur != NULL; cur = cur->next) {
        character = (struct namedlist_t*)cur->node;
        print_character_rival(stdout, world, character);
    }

    return EXIT_SUCCESS;
}
