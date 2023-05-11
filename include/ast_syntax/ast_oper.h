#ifndef AST_OPER_H
#define AST_OPER_H

#include "struct.h"

#define ExprType_U              0x01
#define ExprType_M              0x02
#define ExprType_A              0x03
#define ExprType_Shift          0x04
#define ExprType_And            0x05
#define ExprType_Xor            0x06
#define ExprType_Or             0x07
#define ExprType_Comp           0x08
#define ExprType_NotTest        0x09
#define ExprType_AndTest        0x0a
#define ExprType_XorTest        0x0b
#define ExprType_OrTest         0x0c

int u_oper(struct ast_parser *, struct node_st *);
int m_oper(struct ast_parser *, struct node_st *);
int a_oper(struct ast_parser *, struct node_st *);
int shift_oper(struct ast_parser *, struct node_st *);
int and_oper(struct ast_parser *, struct node_st *);
int xor_oper(struct ast_parser *, struct node_st *);
int or_oper(struct ast_parser *, struct node_st *);

int comp_oper(struct ast_parser *, struct node_st *);
int not_test_oper(struct ast_parser *, struct node_st *);
int and_test_oper(struct ast_parser *, struct node_st *);
int xor_test_oper(struct ast_parser *, struct node_st *);
int or_test_oper(struct ast_parser *, struct node_st *);
int list_oper(struct ast_parser *, struct node_st *, short start, short end);

int list_ident(struct ast_parser *, struct node_st *);

#endif //AST_OPER_H
