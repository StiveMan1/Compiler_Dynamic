#ifndef AST_EXPR_H
#define AST_EXPR_H

#include "struct.h"

#define Literal_None            0x00
#define Literal_String          0x01
#define Literal_Number          0x02

#define PrimType_List           0x01
#define PrimType_Ident_new      0x02
#define PrimType_Ident_get      0x03
#define PrimType_Literal        0x04
#define PrimType_Attrib         0x05
#define PrimType_Subscript      0x06
#define PrimType_Call           0x07
#define PrimType_Tuple           0x08

char scopes_expr(struct ast_parser *, struct node_st *);
char list_expr(struct ast_parser *, struct node_st *);
char ident_get_expr(struct ast_parser *, struct node_st *);
char ident_new_expr(struct ast_parser *, struct node_st *);
char bool_expr(struct ast_parser *, struct node_st *);
char number_expr(struct ast_parser *, struct node_st *);
char string_expr(struct ast_parser *, struct node_st *);
char null_expr(struct ast_parser *, struct node_st *);
char literal_expr(struct ast_parser *, struct node_st *);
char atom_expr(struct ast_parser *, struct node_st *);
char primary_expr(struct ast_parser *, struct node_st *);

#endif //AST_EXPR_H
