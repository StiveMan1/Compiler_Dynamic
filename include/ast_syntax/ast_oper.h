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

char u_oper(struct ast_parser *, struct node_st *);
char m_oper(struct ast_parser *, struct node_st *);
char a_oper(struct ast_parser *, struct node_st *);
char shift_oper(struct ast_parser *, struct node_st *);
char and_oper(struct ast_parser *, struct node_st *);
char xor_oper(struct ast_parser *, struct node_st *);
char or_oper(struct ast_parser *, struct node_st *);

char comp_oper(struct ast_parser *, struct node_st *);
char not_test_oper(struct ast_parser *, struct node_st *);
char and_test_oper(struct ast_parser *, struct node_st *);
char or_test_oper(struct ast_parser *, struct node_st *);
char list_oper(struct ast_parser *, struct node_st *, short start, short end);
char tuple_oper(struct ast_parser *parser, struct node_st *expr, short start, short end);

char list_ident(struct ast_parser *, struct node_st *);

#endif //AST_OPER_H
