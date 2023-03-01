#ifndef AST_STMT_H
#define AST_STMT_H

#include "struct.h"


#define StmtType_If             0x01
#define StmtType_While          0x02
#define StmtType_DoWhile        0x03
#define StmtType_For            0x04
#define StmtType_ForHeader      0x05
#define StmtType_Try            0x06
#define StmtType_Params         0x07
#define StmtType_Func           0x08
#define StmtType_Class          0x09
#define StmtType_Annot          0x0a
#define StmtType_Assign         0x0b
#define StmtType_Return         0x0c
#define StmtType_Break          0x0d
#define StmtType_Continue       0x0e
#define StmtType_Import         0x0f
#define StmtType_List           0x10
#define StmtType_Extends        0x11

void annotation_stmt(struct ast_parser *, struct node_st *);
void assignment_stmt(struct ast_parser *, struct node_st *);
void return_stmt(struct ast_parser *, struct node_st *);
void simple_stmt(struct ast_parser *, struct node_st *);
void stmt_list(struct ast_parser *, struct node_st *);

void for_header1_stmt(struct ast_parser *, struct node_st *);
void for_header2_stmt(struct ast_parser *, struct node_st *);

void for_stmt(struct ast_parser *, struct node_st *);
void if_stmt(struct ast_parser *, struct node_st *);
void while_stmt(struct ast_parser *, struct node_st *);
void do_while_stmt(struct ast_parser *, struct node_st *);
void extends_list(struct ast_parser *, struct node_st *);
void parameter_list(struct ast_parser *, struct node_st *);
void function_stmt(struct ast_parser *, struct node_st *);
void compound_stmt(struct ast_parser *, struct node_st *);

void statement(struct ast_parser *, struct node_st *);
void suite(struct ast_parser *, struct node_st *);

#endif //AST_STMT_H
