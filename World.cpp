//
// Created by Clay on 5/27/2022.
//

#include <fstream>
using std::ifstream;
#include "World.h"

// Forward declarations for functions in adversary.c, from the lemon parser interface
void *ParseAlloc(void *(*mallocProc)(size_t));
void Parse(
        void *yyp,                   /* The parser */
        int yymajor,                 /* The major token code number */
        struct token* yyminor,       /* The value for the token */
        struct world_t*               /* Optional %extra_argument parameter */
);
void ParseFree(
        void *p,                    /* The parser to be deleted */
        void (*freeProc)(void*)     /* Function used to reclaim memory */
);

World::World() {
    world = new_world();
}

World::~World() {
    free(world);
}

void World::read(ifstream& in) {
    lexer lex(in);

    void* parser = ParseAlloc(malloc);
    while(auto tok = lex.get()) {
        Parse(parser, tok->token_type, tok, world);
    }
    Parse(parser, 0, nullptr, world);
    ParseFree(parser, free);
}