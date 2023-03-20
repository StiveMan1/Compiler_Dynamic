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
#define analyze_start size_t current_pointing = parser->position;struct node_st *expr_next = expr; struct token_st *token = NULL;struct object_st *token_ptr = NULL;char result = SN_Status_Nothing, sub_result;
#define analyze_end \
end:    if (result != SN_Status_Success) {node_clear(expr);parser->position = current_pointing;} return result; \
sub:    result = sub_result; goto end; \
eof:    result = SN_Status_EOF; parser->error_pos = parser->position; goto end; \
err:    result = SN_Status_Error; parser->error_pos = parser->position; goto end;

#define check_call(call, check) {sub_result = call; if (sub_result == SN_Status_Nothing) check if (sub_result != SN_Status_Success) goto sub;}

char annotation_stmt(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_VAR)
            goto end;
        parser->position++;

        expr_add(expr)
        check_call(ident_new_expr(parser, expr_next), goto err;)

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Annot;

        parser_end goto eof;
        parser_get
        if (token->type != TokenType_Special || token->subtype != Special_EQ) goto err;
        parser->position++;

        expr_add(expr)
        check_call(or_test_oper(parser, expr_next), goto err;)

        result = SN_Status_Success;
    }
analyze_end
}

char assignment_stmt(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        expr_add(expr)
        check_call(primary_expr(parser, expr_next), goto end;)
        if (expr_next->type != PrimType_Subscript && expr_next->type != PrimType_Ident_get &&
            expr_next->type != PrimType_Attrib)
            goto end;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Assign;

        parser_end goto eof;
        parser_get
        if (token->type != TokenType_Special || token->subtype != Special_EQ) goto end;
        parser->position++;
        array_append(expr->tokens, token_ptr);

        while (parser->position < parser->list->size) {
            expr_add(expr)
            check_call(or_test_oper(parser, expr_next), goto err;)

            parser_end {
                result = SN_Status_Success;
                goto end;
            }
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_EQ) {
                result = SN_Status_Success;
                goto end;
            }
            parser->position++;
            array_append(expr->tokens, token_ptr);

            if (expr_next->type != PrimType_Subscript && expr_next->type != PrimType_Ident_get &&
                expr_next->type != PrimType_Attrib)
                goto err;
        }
        goto err;
    }
analyze_end
}

char return_stmt(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_RETURN) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Return;
        result = SN_Status_Success;
        expr_add(expr)
        printf("");
        check_call(or_test_oper(parser, expr_next), {array_resize(expr->next, expr->next->size - 1);goto end;})

    }
analyze_end
}

char simple_stmt(struct ast_parser *parser, struct node_st *expr) {
    char result = return_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = annotation_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = assignment_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = or_test_oper(parser, expr);
    return result;
}

char stmt_list(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            check_call(simple_stmt(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_COMMA) break;
            parser->position++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast(expr)
                expr->type = StmtType_List;
                expr->main_type = MainType_Stmt;
                times = 1;
            }
            expr_add(expr)
        }
        result = SN_Status_Success;
    }
analyze_end
}

char if_stmt(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_IF) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_If;

        expr_add(expr)
        check_call(or_test_oper(parser, expr_next), goto err;)

        expr_add(expr)
        check_call(suite(parser, expr_next, KeyWord_THEN), goto err;)

        if (parser->list->size > parser->position) {
            parser_get
            if (token->type == TokenType_KeyWords && token->subtype == KeyWord_ELSE) {
                expr_add(expr)
                check_call(suite(parser, expr_next, KeyWord_ELSE), goto err;)
            }
        }
        result = SN_Status_Success;
    }
analyze_end
}

char while_stmt(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_WHILE) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_While;

        expr_add(expr)
        check_call(scopes_expr(parser, expr_next), goto err;)

        expr_add(expr)
        check_call(suite(parser, expr_next, KeyWord_LOOP), goto err;)
        result = SN_Status_Success;
    }
analyze_end
}

char for_stmt(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_FOR) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_For;

        expr_add(expr)
        check_call(ident_new_expr(parser, expr_next), goto err;)

        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_IN) goto err;
        parser->position++;

        expr_add(expr)
        check_call(ident_get_expr(parser, expr_next), goto err;)

        expr_add(expr)
        check_call(suite(parser, expr_next, KeyWord_LOOP), goto err;)
        result = SN_Status_Success;
    }
analyze_end
}

char extends_list(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        check_call(list_oper(parser, expr, Special_LSB, Special_RSB), goto end;)
        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Extends;
        result = SN_Status_Success;
    }
analyze_end
}

char parameter_list(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        check_call(list_ident(parser, expr), goto end;)
        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Params;
        result = SN_Status_Success;
    }
analyze_end
}

char function_stmt(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_FUNC) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Func;

        expr_add(expr)
        check_call(ident_new_expr(parser, expr_next), goto err;)

        expr_add(expr)
        check_call(parameter_list(parser, expr_next), goto err;)

        expr_add(expr)
        check_call(suite(parser, expr_next, KeyWord_IS), goto err;)

        result = SN_Status_Success;
    }
analyze_end
}

char compound_stmt(struct ast_parser *parser, struct node_st *expr) {
    char result = if_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = for_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = while_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = function_stmt(parser, expr);
    return result;
}

char statement(struct ast_parser *parser, struct node_st *expr) {
    char res = compound_stmt(parser, expr);
    if (res != SN_Status_Nothing) return res;
    analyze_start
    {
        sub_result = stmt_list(parser, expr);
        if (sub_result != SN_Status_Nothing && sub_result != SN_Status_Success) goto sub;

        parser_end goto eof;
        parser_get
        if (token->type != TokenType_Special || token->subtype != Special_SEMI){
            if(sub_result == SN_Status_Nothing) goto end;
            else goto err;
        }
        parser->position++;
        result = SN_Status_Success;
    }
analyze_end
}

char suite(struct ast_parser *parser, struct node_st *expr, int type_scope) {
    analyze_start
    {
        parser_end goto eof;
        token = parser->list->data[parser->position]->data;
        if (token->type != TokenType_KeyWords || token->subtype != type_scope) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_List;

        parser_end goto eof;
        parser_get
        if (token->type == TokenType_KeyWords && token->subtype == KeyWord_END) {
            parser->position++;
            result = SN_Status_Success;
            goto end;
        } else if (type_scope == KeyWord_THEN && token->type == TokenType_KeyWords && token->subtype == KeyWord_ELSE) {
            result = SN_Status_Success;
            goto end;
        }

        while (parser->position < parser->list->size) {
            expr_add(expr)
            check_call(statement(parser, expr_next), goto err;)

            parser_end break;
            parser_get
            if (token->type == TokenType_KeyWords && token->subtype == KeyWord_END) {
                parser->position++;
                result = SN_Status_Success;
                goto end;
            } else if (type_scope == KeyWord_THEN && token->type == TokenType_KeyWords &&
                       token->subtype == KeyWord_ELSE) {
                result = SN_Status_Success;
                goto end;
            }
        }
        goto err;
    }
analyze_end
}

char token_analyzer(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        expr->main_type = MainType_Stmt;
        expr->type = StmtType_List;

        while (parser->position < parser->list->size) {
            expr_add(expr)
            statement(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;
        }
        result = SN_Status_Success;
    }
analyze_end
}

#pragma clang diagnostic pop