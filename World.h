//
// Created by Clay on 5/27/2022.
//

#ifndef ADVERSARIES_WORLD_H
#define ADVERSARIES_WORLD_H

#include "ast.h"
#include "lexer.h"

#include <string>
using std::string;

class World {

public:
    struct world_t* world;

    World();
    ~World();
    void read(ifstream&);
};


#endif //ADVERSARIES_WORLD_H
