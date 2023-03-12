#ifndef AST_PARSER_H
#define AST_PARSER_H

#include "struct.h"
#include "la_parser.h"

#define MaxBracketNesting 256

struct ast_parser{
    size_t position;
    size_t error_pos;
    struct array_st *list;
};

struct ast_parser *ast_parser_new();
void ast_parser_set(struct ast_parser *, const struct ast_parser *);
void ast_parser_clear(struct ast_parser *);
void ast_parser_free(struct ast_parser *);

void ast_parser_set_list(struct ast_parser *, struct la_parser *);


#endif //AST_PARSER_H
