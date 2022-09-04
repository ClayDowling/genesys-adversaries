#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "token.h"

struct lex_context;

/**
 * @brief Set up a lexer for the given file.
 *
 * @param filename Source file to scan
 * @return struct lex_context* Argument to be passed to lex_scan
 */
struct lex_context *lex_file(const char *filename);

/**
 * @brief Set up a lexer for the given file handle
 * 
 * @param file File handle to parse
 * @return struct lex_context* Argument to be passed to lex_scan
 */
struct lex_context *lex_FILE(FILE* file, const char* name);

/**
 * @brief Release resources associated with the lexer.
 *
 * @param ctx Lexer to finalize
 */
void lex_complete(struct lex_context *ctx);

/**
 * @brief Convert the input stream into the next token.
 *
 * Call lex_scan repeatedly to get tokens, until a NULL is returned, signifying
 * the end of input.
 *
 * @param ctx 				A context previously set up with
 * lex_file
 * @return struct token* 	Next token, or NULL when input is exhausted.
 */
struct token *lex_scan(struct lex_context *ctx);

/**
 * @brief Add a search directory to scan for use files
 *
 * @param directory
 */
void lex_add_directory(const char* directory);

struct lex_search_path {
    const char *folder;
    struct lex_search_path *next;
};

extern struct lex_search_path *SEARCH_PATH;
#endif