#ifndef AST_NODE_H
#define AST_NODE_H

#include "struct.h"

#define ExprType_None           0x00

#define MainType_None           0x00
#define MainType_Expr           0x01
#define MainType_Oper           0x02
#define MainType_Stmt           0x03

struct node_st{
    short main_type;
    short type;

    struct object_st *data;
    struct array_st *next;
    struct array_st *tokens;
};


struct node_st *node_new();
void node_set(struct node_st *, const struct node_st *);
void node_clear(struct node_st *);
void node_free(struct node_st *);

#endif //AST_NODE_H
