#ifndef LA_PARSER_H
#define LA_PARSER_H

#include "struct.h"

#define MaxBracketNesting 256

struct la_parser{
    char *data;
    size_t position;
    size_t str_size;

    size_t current_line;

    char scope_buf[MaxBracketNesting];
    size_t scope_pos;

    struct array_st *list;
};

struct la_parser *la_parser_new();
void la_parser_set(struct la_parser *, const struct la_parser *);
void la_parser_clear(struct la_parser *);
void la_parser_free(struct la_parser *);

void la_parser_set_str(struct la_parser *, char *, size_t);
void la_parser_set_file(struct la_parser *, char *);

#endif //LA_PARSER_H
