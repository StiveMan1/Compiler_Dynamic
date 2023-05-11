#ifndef AST_NODE_H
#define AST_NODE_H

#include "struct.h"

#define ExprType_None           0x00

#define MainType_None           0x00
#define MainType_Expr           0x01
#define MainType_Oper           0x02
#define MainType_Stmt           0x03
#define MainType_Type           0x04

struct node_st{
    short main_type;
    short type;

    struct object_st *data;
    struct object_st *closure;

    struct array_st *next;
    struct array_st *tokens;

    size_t pos, line_num, line_pos;
};


struct node_st *node_new();
void node_set(struct node_st *, const struct node_st *);
void node_clear(struct node_st *);
void node_free(struct node_st *);

void node_set_position(struct node_st *, struct token_st *);

#endif //AST_NODE_H
