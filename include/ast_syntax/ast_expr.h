#ifndef AST_EXPR_H
#define AST_EXPR_H

#include "struct.h"

#define PrimType_List           0x01
#define PrimType_Ident_new      0x02
#define PrimType_Ident_get      0x03
#define PrimType_Literal        0x04
#define PrimType_Attrib         0x05
#define PrimType_Subscript      0x06
#define PrimType_Call           0x07
#define PrimType_Tuple          0x08

void set_error(struct ast_parser *, size_t, char *);

int scopes_expr(struct ast_parser *, struct node_st *);
int list_expr(struct ast_parser *, struct node_st *);
int ident_get_expr(struct ast_parser *, struct node_st *);
int ident_new_expr(struct ast_parser *, struct node_st *);
int bool_expr(struct ast_parser *, struct node_st *);
int number_expr(struct ast_parser *, struct node_st *);
int string_expr(struct ast_parser *, struct node_st *);
int null_expr(struct ast_parser *, struct node_st *);
int literal_expr(struct ast_parser *, struct node_st *);
int atom_expr(struct ast_parser *, struct node_st *);
int primary_expr(struct ast_parser *, struct node_st *);

#endif //AST_EXPR_H
