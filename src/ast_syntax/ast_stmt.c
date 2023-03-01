#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

#include <stdio.h>
#include "ast_analyze.h"
#include "lexical_analysis.h"

#define expr_cast(expr) { struct object_st *obj = object_new(); object_set_type(obj, NODE_TYPE); \
node_set(obj->data, expr); node_clear(expr); array_append((expr)->next, obj); object_free(obj); }
#define expr_add(expr) { struct object_st *obj = object_new(); object_set_type(obj, NODE_TYPE); \
array_append((expr)->next, obj); expr_next = obj->data; object_free(obj);}
#define parser_end if (parser->list->size <= parser->position)
#define parser_get token_ptr = parser->list->data[parser->position];token = parser->list->data[parser->position]->data;
#define analyze_end if (!result) {node_clear(expr);parser->position = current_pointing;}
#define analyze_start size_t current_pointing = parser->position;struct node_st *expr_next = expr; \
struct token_st *token = NULL;struct object_st *token_ptr = NULL;int result = 0;

void annotation_stmt(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_VAR)
            goto end;
        parser->position++;


        expr_add(expr)
        ident_new_expr(parser, expr_next);
        if (expr_next->type == ExprType_None)
            goto end;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Annot;

        parser_end goto end;
        parser_get
        if (token->type != TokenType_Special || token->subtype != Special_EQ)
            goto end;
        parser->position++;

        expr_add(expr)
        or_test_oper(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        result = 1;
    }
    end:
    analyze_end
}
void assignment_stmt(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        expr_add(expr)
        primary_expr(parser, expr_next);
        if (expr_next->type != PrimType_Subscript && expr_next->type != PrimType_Ident_get &&
            expr_next->type != PrimType_Attrib)
            goto end;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Assign;

        parser_end goto end;
        parser_get
        if (token->type != TokenType_Special || token->subtype != Special_EQ)
            goto end;
        parser->position++;
        array_append(expr->tokens, token_ptr);

        while (parser->position < parser->list->size) {
            expr_add(expr)
            or_test_oper(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;

            parser_end {
                result = 1;
                goto end;
            }
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_EQ) {
                result = 1;
                goto end;
            }
            parser->position++;
            array_append(expr->tokens, token_ptr);

            if (expr_next->type != PrimType_Subscript && expr_next->type != PrimType_Ident_get &&
                expr_next->type != PrimType_Attrib)
                goto end;
        }
    }
    end:
    analyze_end
}

void return_stmt(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_RETURN) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Return;

        parser_end goto end;
        parser_get
        if (token->type == TokenType_Special && token->subtype == Special_SEMI){
            result = 1;
            goto end;
        }

        expr_add(expr)
        while (parser->position < parser->list->size) {
            and_test_oper(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;

            parser_end break;
            parser_get
            if (token->type == TokenType_Special && token->subtype == Special_SEMI){
                result = 1;
                goto end;
            }
            if (token->type != TokenType_Special || token->subtype != Special_COMMA) break;
            parser->position++;

            parser_end break;
            expr_add(expr)
        }
    }
    end:
    analyze_end
}

void simple_stmt(struct ast_parser *parser, struct node_st *expr) {
    return_stmt(parser, expr);
    if (expr->type != ExprType_None) return;
    annotation_stmt(parser, expr);
    if (expr->type != ExprType_None) return;
    assignment_stmt(parser, expr);
    if (expr->type != ExprType_None) return;
    or_test_oper(parser, expr);
}

void stmt_list(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            simple_stmt(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_COMMA) break;
            parser->position++;

            parser_end goto end;
            if (times == 0) {
                expr_cast(expr)
                expr->type = StmtType_List;
                expr->main_type = MainType_Stmt;
                times = 1;
            }
            expr_add(expr)
            array_append(expr->tokens, token_ptr);
        }
        result = 1;
    }
    end:
    analyze_end
}

void for_header1_stmt(struct ast_parser *parser, struct node_st *expr) {
    //TODO
}

void for_header2_stmt(struct ast_parser *parser, struct node_st *expr) {
    //TODO
}

void for_stmt(struct ast_parser *parser, struct node_st *expr) {
    // TODO
}

void if_stmt(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_IF) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_If;

        expr_add(expr)
        scopes_expr(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        expr_add(expr)
        suite(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        while (parser->position < parser->list->size) {
            parser_get
            if (token->type != TokenType_KeyWords || token->subtype != KeyWord_ELSE) break;
            parser->position++;

            parser_end goto end;
            parser_get
            if (token->type == TokenType_KeyWords && token->subtype == KeyWord_IF) {
                parser->position++;

                expr_add(expr)
                scopes_expr(parser, expr_next);
                if (expr_next->type == ExprType_None) goto end;

                expr_add(expr)
                suite(parser, expr_next);
                if (expr_next->type == ExprType_None) goto end;
            } else {
                expr_add(expr)
                suite(parser, expr_next);
                if (expr_next->type == ExprType_None) goto end;
            }
        }
        result = 1;
    }
    end:
    analyze_end
}

void while_stmt(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_WHILE) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_While;

        expr_add(expr)
        scopes_expr(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        expr_add(expr)
        suite(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;
        result = 1;
    }
    end:
    analyze_end
}

void do_while_stmt(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_LOOP) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_DoWhile;

        expr_add(expr)
        suite(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_WHILE) goto end;
        parser->position++;

        expr_add(expr)
        scopes_expr(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        result = 1;
    }
    end:
    analyze_end
}

void extends_list(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        list_oper(parser, expr, Special_LSB, Special_RSB);
        if (expr->type != PrimType_List) goto end;
        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Extends;
        result = 1;
    }
    end:
    analyze_end
}

void parameter_list(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        list_oper(parser, expr, Special_LSB, Special_RSB);
        if (expr->type != PrimType_List) goto end;
        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Params;
        result = 1;
    }
    end:
    analyze_end
}

void function_stmt(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_FUNC) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Func;

        expr_add(expr)
        ident_new_expr(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        expr_add(expr)
        parameter_list(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        expr_add(expr)
        suite(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        result = 1;
    }
    end:
    analyze_end
}

void compound_stmt(struct ast_parser *parser, struct node_st *expr) {
    for_stmt(parser, expr);
    if (expr->type != ExprType_None) return;
    if_stmt(parser, expr);
    if (expr->type != ExprType_None) return;
    while_stmt(parser, expr);
    if (expr->type != ExprType_None) return;
    do_while_stmt(parser, expr);
    if (expr->type != ExprType_None) return;
    function_stmt(parser, expr);

}

void statement(struct ast_parser *parser, struct node_st *expr) {
    compound_stmt(parser, expr);
    if (expr->type != ExprType_None) return;
    analyze_start
    {
        stmt_list(parser, expr);

        parser_end goto end;
        parser_get
        if (token->type != TokenType_Special || token->subtype != Special_SEMI) goto end;
        parser->position++;
        result = 1;
    }
    end:
    analyze_end
}

void suite(struct ast_parser *parser, struct node_st *expr) {
    statement(parser, expr);
    if (expr->type != ExprType_None) return;
    analyze_start
    {
        parser_end goto end;
        token = parser->list->data[parser->position]->data;
        if (token->type != TokenType_Special || token->subtype != Special_LCB) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_List;

        parser_end goto end;
        token = parser->list->data[parser->position]->data;
        if (token->type == TokenType_Special && token->subtype == Special_RCB) {
            parser->position++;
            result = 1;
            goto end;
        }

        expr_add(expr)
        while (parser->position < parser->list->size) {
            statement(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;

            parser_end break;
            parser_get
            if (token->type == TokenType_Special && token->subtype == Special_RCB) {
                parser->position++;
                result = 1;
                goto end;
            }

            parser_end break;
            expr_add(expr)
        }
    }
    end:
    analyze_end
}

void token_analyzer(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        expr->main_type = MainType_Stmt;
        expr->type = StmtType_List;

        expr_add(expr)
        while (parser->position < parser->list->size) {
            suite(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;
            parser_end break;
            expr_add(expr)
        }
        result = 1;
    }
    end:
    analyze_end
}

#pragma clang diagnostic pop