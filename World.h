//
// Created by Clay on 5/27/2022.
//

#ifndef ADVERSARIES_WORLD_H
#define ADVERSARIES_WORLD_H

extern "C" {
#include "ast.h"
}

#include "lexer.h"

#include <string>
using std::string;

#include <iostream>
using std::istream;

class World {

public:
    struct world_t* world;

    World();
    ~World();
    void read(istream&);
};


#endif //ADVERSARIES_WORLD_H
