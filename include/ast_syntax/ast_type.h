#ifndef COMPILER_DYNAMIC_AST_TYPE_H
#define COMPILER_DYNAMIC_AST_TYPE_H

#include "struct.h"

#define Type_int            0x01
#define Type_real           0x02
#define Type_bool           0x03
#define Type_string         0x04
#define Type_array          0x05
#define Type_tuple          0x06
#define Type_func           0x07

int type_expr(struct ast_parser *, struct node_st *);
int type_int(struct ast_parser *, struct node_st *);
int type_real(struct ast_parser *, struct node_st *);
int type_bool(struct ast_parser *, struct node_st *);
int type_string(struct ast_parser *, struct node_st *);
int type_array(struct ast_parser *, struct node_st *);
int type_tuple(struct ast_parser *, struct node_st *);
int type_func(struct ast_parser *, struct node_st *);

#endif //COMPILER_DYNAMIC_AST_TYPE_H
