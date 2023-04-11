#ifndef AST_STMT_H
#define AST_STMT_H

#include "struct.h"


#define StmtType_If             0x01
#define StmtType_While          0x02
#define StmtType_Decl           0x03
#define StmtType_For            0x04
#define StmtType_Try            0x06
#define StmtType_Params         0x07
#define StmtType_Func           0x08
#define StmtType_Func_Body      0x09
#define StmtType_Annot          0x0a
#define StmtType_Assign         0x0b
#define StmtType_Return         0x0c
#define StmtType_List           0x10
#define StmtType_Print          0x12

int annotation_stmt(struct ast_parser *, struct node_st *);
int declaration_stmt(struct ast_parser *, struct node_st *);
int assignment_stmt(struct ast_parser *, struct node_st *);
int return_stmt(struct ast_parser *, struct node_st *);
int print_stmt(struct ast_parser *, struct node_st *);
int simple_stmt(struct ast_parser *, struct node_st *);
int stmt_list(struct ast_parser *, struct node_st *);

int for_stmt(struct ast_parser *, struct node_st *);
int if_stmt(struct ast_parser *, struct node_st *);
int while_stmt(struct ast_parser *, struct node_st *);
int parameter_list(struct ast_parser *, struct node_st *);
int function_stmt(struct ast_parser *, struct node_st *);
int compound_stmt(struct ast_parser *, struct node_st *);

int statement(struct ast_parser *, struct node_st *);
int func_body(struct ast_parser *, struct node_st *);
int body(struct ast_parser *, struct node_st *, int);

#endif //AST_STMT_H
