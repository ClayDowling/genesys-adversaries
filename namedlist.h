//
// Created by Clay on 1/20/2023.
//

#ifndef ADVERSARIES_NAMEDLIST_H
#define ADVERSARIES_NAMEDLIST_H

#include "ast.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

class namedlist {

public:
    enum namedlist_type type;
    string name;
    vector<struct listitem_t> contents;
};

#endif //ADVERSARIES_NAMEDLIST_H
