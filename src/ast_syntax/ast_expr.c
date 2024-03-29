#include "ast_analyze.h"

// Error
void set_error(struct ast_parser *parser, size_t position, char *msg) {
    error_fill_in(
            parser->error_obj,
            SYNTAX_ANALYSIS_ERROR,
            msg,
            ((struct token_st *)parser->list->data[position]->data)->line_pos,
            ((struct token_st *)parser->list->data[position]->data)->line_num,
            ((struct token_st *)parser->list->data[position]->data)->pos);
}

#define expr_cast(expr) { struct object_st *obj = object_new(); object_set_type(obj, NODE_TYPE); \
node_set(obj->data, expr); node_clear(expr); array_append((expr)->next, obj); object_free(obj); }
#define expr_add(expr) { struct object_st *obj = object_new(); object_set_type(obj, NODE_TYPE); \
array_append((expr)->next, obj); expr_next = obj->data; object_free(obj);}
#define parser_end if (parser->list->size <= parser->position)
#define parser_get token_ptr = parser->list->data[parser->position];token = parser->list->data[parser->position]->data;
#define analyze_start size_t current_pointing = parser->position;struct node_st *expr_next = expr; struct token_st *token = NULL;struct object_st *token_ptr = NULL;int result = SN_Status_Nothing, sub_result;
#define analyze_end \
end:    if (result != SN_Status_Success) {node_clear(expr);parser->position = current_pointing;} return result; \
sub:    result = sub_result; goto end; \
eof:    result = SN_Status_EOF; set_error(parser, parser->position - 1, ""); goto end; \
err:    result = SN_Status_Error; set_error(parser, parser->position, "Unexpected end of file"); goto end;

#define check_call(call, check) {sub_result = call; if (sub_result == SN_Status_Nothing) check if (sub_result != SN_Status_Success) goto sub;}

// Scopes expressions
int scopes_expr(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        token = parser->list->data[parser->position]->data;
        if (token->type != TokenType_Special || token->subtype != Special_LSB) goto end;
        parser->position++;

        check_call(or_test_oper(parser, expr), goto err;)

        parser_end goto eof;
        token = parser->list->data[parser->position]->data;
        if (token->type != TokenType_Special || token->subtype != Special_RSB) goto err;
        parser->position++;

        result = SN_Status_Success;
    }
analyze_end
}
// Tuple expressions
int tuple_expr(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_Special || token->subtype != Special_LCB) goto end;
        parser->position++;

        expr->main_type = MainType_Expr;
        node_set_position(expr, parser->list->data[parser->position - 1]->data);
        expr->type = PrimType_Tuple;

        parser_end goto eof;
        parser_get
        if (token->type == TokenType_Special && token->subtype == Special_RCB) {
            parser->position++;
            result = SN_Status_Success;
            goto end;
        }

        while (parser->position < parser->list->size) {
            expr_add(expr)

            check_call(annotation_stmt(parser, expr_next), goto err;)
            parser_end goto err;
            parser_get
            if (token->type == TokenType_Special && token->subtype == Special_RCB) {
                parser->position++;
                result = SN_Status_Success;
                goto end;
            }
            parser_end goto err;
            if (token->type != TokenType_Special || token->subtype != Special_COMMA) break;
            parser->position++;
        }
        goto err;
    }
analyze_end
}
// List expressions
int list_expr(struct ast_parser *parser, struct node_st *expr) {
    return list_oper(parser, expr, Special_LSQB, Special_RSQB);
}
// Identifier new expressions
int ident_new_expr(struct ast_parser *parser, struct node_st *expr) {
    parser_end return SN_Status_EOF;
    struct token_st *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_Identifier) return SN_Status_Nothing;
    parser->position++;
    expr->type = PrimType_Ident_new;
    expr->main_type = MainType_Expr;
    node_set_position(expr, parser->list->data[parser->position - 1]->data);
    expr->data = object_new();
    object_set_type(expr->data, STRING_TYPE);
    string_set_str(expr->data->data, token->data, token->size);
    return SN_Status_Success;
}
// Identifier get expressions
int ident_get_expr(struct ast_parser *parser, struct node_st *expr) {
    parser_end return SN_Status_EOF;
    struct token_st *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_Identifier) return SN_Status_Nothing;
    parser->position++;
    expr->type = PrimType_Ident_get;
    expr->main_type = MainType_Expr;
    node_set_position(expr, parser->list->data[parser->position - 1]->data);
    expr->data = object_new();
    object_set_type(expr->data, STRING_TYPE);
    string_set_str(expr->data->data, token->data, token->size);
    return SN_Status_Success;
}
// Bool expressions
int bool_expr(struct ast_parser *parser, struct node_st *expr) {
    parser_end return SN_Status_EOF;
    struct token_st *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_KeyWords || (token->subtype != KeyWord_FALSE && token->subtype != KeyWord_TRUE))
        return SN_Status_Nothing;
    parser->position++;
    expr->type = PrimType_Literal;
    expr->main_type = MainType_Expr;
    node_set_position(expr, parser->list->data[parser->position - 1]->data);
    expr->data = object_new();
    object_set_type(expr->data, BOOL_TYPE);
    if (token->subtype == KeyWord_FALSE) ((struct bool_st *)expr->data->data)->data = 0;
    else ((struct bool_st *)expr->data->data)->data = 1;
    return SN_Status_Success;
}
// Number expressions: float, integer
int number_expr(struct ast_parser *parser, struct node_st *expr) {
    parser_end return SN_Status_EOF;
    struct token_st *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_Int) return SN_Status_Nothing;
    parser->position++;
    expr->type = PrimType_Literal;
    expr->main_type = MainType_Expr;
    node_set_position(expr, parser->list->data[parser->position - 1]->data);
    expr->data = object_new();
    if(token->subtype == IntType_float) {
        object_set_type(expr->data, REAL_TYPE);
        ((struct real_st *)expr->data->data)->data = strtof(token->data, NULL);
    } else {
        object_set_type(expr->data, INTEGER_TYPE);
        if (token->subtype == IntType_bin)
            ((struct integer_st *) expr->data->data)->data = strtol(token->data, NULL, 2);
        else if (token->subtype == IntType_hex)
            ((struct integer_st *) expr->data->data)->data = strtol(token->data, NULL, 16);
        else if (token->subtype == IntType_oct)
            ((struct integer_st *) expr->data->data)->data = strtol(token->data, NULL, 8);
        else if (token->subtype == IntType_dec)
            ((struct integer_st *) expr->data->data)->data = strtol(token->data, NULL, 10);
    }
    return SN_Status_Success;
}
// String expressions
int string_expr(struct ast_parser *parser, struct node_st *expr) {
    parser_end return SN_Status_EOF;
    struct token_st *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_String) return SN_Status_Nothing;
    parser->position++;
    expr->type = PrimType_Literal;
    expr->main_type = MainType_Expr;
    node_set_position(expr, parser->list->data[parser->position - 1]->data);
    expr->data = object_new();
    object_set_type(expr->data, STRING_TYPE);
    string_set_str(expr->data->data, token->data, token->size);
    return SN_Status_Success;
}
// Null expression
int null_expr(struct ast_parser *parser, struct node_st *expr) {
    parser_end return SN_Status_EOF;
    struct token_st *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_KeyWords || token->subtype != KeyWord_EMPTY) return SN_Status_Nothing;
    parser->position++;
    expr->type = PrimType_Literal;
    expr->main_type = MainType_Expr;
    node_set_position(expr, parser->list->data[parser->position - 1]->data);
    expr->data = object_new();
    return SN_Status_Success;
}
// Literal expressions
int literal_expr(struct ast_parser *parser, struct node_st *expr) {
    int result = null_expr(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = string_expr(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = number_expr(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = bool_expr(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = list_expr(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = tuple_expr(parser, expr);
    return result;
}
// Lambda expressions
int lambda_expr(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_FUNC) goto end;
        parser->position++;

        expr->main_type = MainType_Expr;
        node_set_position(expr, parser->list->data[parser->position - 1]->data);
        expr->type = PrimType_Lambda;

        expr_add(expr)
        check_call(func_body(parser, expr_next), goto err;)

        result = SN_Status_Success;
    }
analyze_end
}
// Atom expressions
int atom_expr(struct ast_parser *parser, struct node_st *expr) {
    int result = ident_get_expr(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = literal_expr(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = scopes_expr(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = lambda_expr(parser, expr);
    return result;
}
// Primary expressions
int primary_expr(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        check_call(atom_expr(parser, expr_next), goto end;)
        while(parser->position < parser->list->size){
            token = parser->list->data[parser->position]->data;
            if (token->type == TokenType_Special && token->subtype == Special_DOT) {
                parser->position++;

                expr_cast(expr)
                expr->type = PrimType_Attrib;
                expr->main_type = MainType_Expr;
                node_set_position(expr, parser->list->data[parser->position - 1]->data);
                expr_add(expr)

                parser_end goto eof;
                parser_get
                if (token->type != TokenType_Identifier && (token->type != TokenType_Int || token->subtype == IntType_float)) goto err;
                parser->position++;

                expr->data = object_new();
                if (token->type == TokenType_Identifier) {
                    object_set_type(expr->data, STRING_TYPE);
                    string_set_str(expr->data->data, token->data, token->size);
                } else {
                    object_set_type(expr->data, INTEGER_TYPE);
                    if (token->subtype == IntType_bin)
                        ((struct integer_st *) expr->data->data)->data = strtol(token->data, NULL, 2);
                    else if (token->subtype == IntType_hex)
                        ((struct integer_st *) expr->data->data)->data = strtol(token->data, NULL, 16);
                    else if (token->subtype == IntType_oct)
                        ((struct integer_st *) expr->data->data)->data = strtol(token->data, NULL, 8);
                    else if (token->subtype == IntType_dec)
                        ((struct integer_st *) expr->data->data)->data = strtol(token->data, NULL, 10);
                }
                continue;
            } // PrimType_Attrib
            else if (token->type == TokenType_Special && token->subtype == Special_LSQB) {
                parser->position++;

                expr_cast(expr)
                expr->type = PrimType_Subscript;
                expr->main_type = MainType_Expr;
                node_set_position(expr, parser->list->data[parser->position - 1]->data);
                expr_add(expr)

                check_call(or_test_oper(parser, expr_next), goto err;)

                parser_end goto eof;
                token = parser->list->data[parser->position]->data;
                if (token->type != TokenType_Special || token->subtype != Special_RSQB) goto err;
                parser->position++;

                continue;
            } // PrimType_Subscript
            else if (token->type == TokenType_Special && token->subtype == Special_LSB) {
                expr_cast(expr)
                expr->type = PrimType_Call;
                expr->main_type = MainType_Expr;
                node_set_position(expr, parser->list->data[parser->position - 1]->data);
                expr_add(expr)

                check_call(list_oper(parser, expr_next, Special_LSB, Special_RSB), goto err;)

                continue;
            } // PrimType_Call
            else if (token->type == TokenType_KeyWords && token->subtype == KeyWord_IS) {
                parser->position++;
                expr_cast(expr)
                expr->type = PrimType_Is;
                expr->main_type = MainType_Expr;
                node_set_position(expr, parser->list->data[parser->position - 1]->data);
                expr_add(expr)

                check_call(type_expr(parser, expr_next), goto err;)

                continue;
            }
            break;
        }
        result = SN_Status_Success;
    }
analyze_end
}