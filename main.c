#include <getopt.h>
#include <limits.h>
#include <locale.h>
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

void usage(void) {
    about();
    fprintf(stderr, "usage: adversaries [-j] game.adv\n"
                    "\n"
                    "-d          Debug information\n"
                    "-j          Print characters as json\n"
                    "-I <path>   Include libraries from path\n"
                    "-h          Print this help message\n"
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

int main(int argc, char* const* argv) {

    if (argc == 1) {
        about();
        return EXIT_FAILURE;
    }

    setlocale(LC_ALL, "");

    void (*printer)(FILE *out, struct world_t*, struct namedlist_t*) = print_character_rival;
    bool debug = false;

    int ch;
    while((ch = getopt(argc, argv, "djhHI:")) != -1) {
        switch(ch) {
            case 'd':
                debug = true;
                break;
            case 'j':
                printer = print_character_json;
                break;
            case 'I':
                lex_add_directory(optarg);
                break;
            case 'h':
            case 'H':
            case '?':
            default:
                usage();
                exit(1);
        }
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

    if (debug) {
        for(struct lex_search_path* cur = SEARCH_PATH; cur != NULL; cur = cur->next) {
            printf("Path: %s\n", cur->folder);
        }
    }

    if (optind > argc) {
        usage();
        fputs("Expected adversary file name", stderr);
        exit(1);
    }

    struct world_t *world = parse_file(argv[optind]);
    const struct namedlist_t **character = world_get_characters(world);
    for(int i = 0; character[i]; ++i ) {
        printer(stdout, world, character[i]);
    }

    return EXIT_SUCCESS;
}
