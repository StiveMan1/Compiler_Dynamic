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
#define StmtType_Print          0x12

char annotation_stmt(struct ast_parser *, struct node_st *);
char assignment_stmt(struct ast_parser *, struct node_st *);
char return_stmt(struct ast_parser *, struct node_st *);
char print_stmt(struct ast_parser *, struct node_st *);
char simple_stmt(struct ast_parser *, struct node_st *);
char stmt_list(struct ast_parser *, struct node_st *);

char for_header1_stmt(struct ast_parser *, struct node_st *);
char for_header2_stmt(struct ast_parser *, struct node_st *);

char for_stmt(struct ast_parser *, struct node_st *);
char if_stmt(struct ast_parser *, struct node_st *);
char while_stmt(struct ast_parser *, struct node_st *);
char parameter_list(struct ast_parser *, struct node_st *);
char function_stmt(struct ast_parser *, struct node_st *);
char compound_stmt(struct ast_parser *, struct node_st *);

char statement(struct ast_parser *, struct node_st *);
char func_body(struct ast_parser *, struct node_st *);
char body(struct ast_parser *, struct node_st *, int);

#endif //AST_STMT_H
