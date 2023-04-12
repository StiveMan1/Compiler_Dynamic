#ifndef LA_PARSER_H
#define LA_PARSER_H

#include "basic.h"

#define MaxBracketNesting 256

// The actual Parser utility for Lexical Analysis.
//   *data        -> pointer to the array of chars (the code)
//   position     -> current processing symbol
//   str_size     -> the total length
//   current_line -> current line, can be useful for error detection
//   scope_buf    -> buffer for scopes. For tracking scopes sequence
//   scope_pos    -> position of current scope
//   *list        -> list of resulting tokens
struct la_parser{
    char *data;
    size_t position;
    size_t str_size;

    size_t current_line;
    size_t line_pos;

    char scope_buf[MaxBracketNesting];
    size_t scope_pos;

    struct array_st *list;

    struct string_st *error_msg;
};

struct la_parser *la_parser_new();
void la_parser_set(struct la_parser *, const struct la_parser *);
void la_parser_clear(struct la_parser *);
void la_parser_free(struct la_parser *);

void la_parser_set_str(struct la_parser *, char *, size_t);
void la_parser_set_file(struct la_parser *, char *);

#endif //LA_PARSER_H
