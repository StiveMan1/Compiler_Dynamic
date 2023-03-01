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
#define ExprType_OrTest         0x0b

void u_oper(struct ast_parser *, struct node_st *);
void m_oper(struct ast_parser *, struct node_st *);
void a_oper(struct ast_parser *, struct node_st *);
void shift_oper(struct ast_parser *, struct node_st *);
void and_oper(struct ast_parser *, struct node_st *);
void xor_oper(struct ast_parser *, struct node_st *);
void or_oper(struct ast_parser *, struct node_st *);

void comp_oper(struct ast_parser *, struct node_st *);
void not_test_oper(struct ast_parser *, struct node_st *);
void and_test_oper(struct ast_parser *, struct node_st *);
void or_test_oper(struct ast_parser *, struct node_st *);
void list_oper(struct ast_parser *, struct node_st *, short start, short end);

#endif //AST_OPER_H
