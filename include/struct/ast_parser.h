#ifndef AST_PARSER_H
#define AST_PARSER_H

#include "struct.h"
#include "la_parser.h"

#define MaxBracketNesting 256

#define ScopeType_None  0x00
#define ScopeType_Func  0x01
#define ScopeType_Loop  0x02

#define SemanticError_Ident     0x01
#define SemanticError_Return    0x02

struct ast_parser{
    size_t position;
    struct array_st *list;
    struct error_st *error_obj;

    struct node_st *tree;

    struct array_st *memory_frame;
    struct array_st *memory_closure;
    struct array_st *expr_stack;

    int type;
};

struct ast_parser *ast_parser_new();
void ast_parser_set(struct ast_parser *, const struct ast_parser *);
void ast_parser_clear(struct ast_parser *);
void ast_parser_free(struct ast_parser *);

void ast_parser_set_list(struct ast_parser *, struct la_parser *);
void ast_parser_get_error(struct ast_parser *, struct error_st *);

void ast_parser_save_type(struct ast_parser *, struct node_st *);
void ast_parser_remove_frame(struct ast_parser *);

struct object_st *ast_parser_set_ident(struct ast_parser *, struct object_st *);
struct object_st *ast_parser_get_ident(struct ast_parser *, struct object_st *);


#endif //AST_PARSER_H
