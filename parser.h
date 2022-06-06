#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

struct world_t* parse_file(const char* filename);
struct world_t* parse_buffer(const char* buffer);

#endif