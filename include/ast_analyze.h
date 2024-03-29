#ifndef AST_SYNTAX_H
#define AST_SYNTAX_H

#include "lexical_analysis.h"

#include "ast_syntax/ast_expr.h"
#include "ast_syntax/ast_oper.h"
#include "ast_syntax/ast_stmt.h"
#include "ast_syntax/ast_type.h"

#define SN_Status_Success   0
#define SN_Status_Nothing   1
#define SN_Status_EOF       2
#define SN_Status_Error     3

int token_analyzer(struct ast_parser *, struct node_st *);

#endif //AST_SYNTAX_H
