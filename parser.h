#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

void * parse_file(const char* filename);
void * parse_buffer(const char* buffer);

#endif