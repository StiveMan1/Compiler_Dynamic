#ifndef AST_SYNTAX_H
#define AST_SYNTAX_H

#include "ast_syntax/ast_expr.h"
#include "ast_syntax/ast_oper.h"
#include "ast_syntax/ast_stmt.h"

void token_analyzer(struct ast_parser *, struct node_st *);

#endif //AST_SYNTAX_H
