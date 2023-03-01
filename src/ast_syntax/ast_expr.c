#include "ast_analyze.h"
#include "lexical_analysis.h"

#define expr_cast(expr) { struct object_st *obj = object_new(); object_set_type(obj, NODE_TYPE); \
node_set(obj->data, expr); node_clear(expr); array_append((expr)->next, obj); object_free(obj); }
#define expr_add(expr) { struct object_st *obj = object_new(); object_set_type(obj, NODE_TYPE); \
array_append((expr)->next, obj); expr_next = obj->data; object_free(obj);}
#define parser_end if (parser->list->size <= parser->position)

void scopes_expr(struct ast_parser *parser, struct node_st *expr) {
    size_t current_pointing = parser->position;
    struct token_st *token = NULL;
    int result = 0;
    {
        parser_end goto end;
        token = parser->list->data[parser->position]->data;
        if (token->type != TokenType_Special || token->subtype != Special_LSB) goto end;
        parser->position++;

        or_test_oper(parser, expr);
        if (expr->type == ExprType_None) goto end;

        parser_end goto end;
        token = parser->list->data[parser->position]->data;
        if (token->type != TokenType_Special || token->subtype != Special_RSB) goto end;
        parser->position++;

        result = 1;
    }
    end:
    if (!result) {
        node_clear(expr);
        parser->position = current_pointing;
    }
}
void list_expr(struct ast_parser *parser, struct node_st *expr) {
    list_oper(parser, expr, Special_LSQB, Special_RSQB);
}
void ident_new_expr(struct ast_parser *parser, struct node_st *expr) {
    parser_end return;
    struct token_st *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_Identifier) return;
    parser->position++;
    expr->type = PrimType_Ident_new;
    expr->main_type = MainType_Expr;
    expr->data = object_new();
    object_set_type(expr->data, STRING_TYPE);
    string_set_str(expr->data->data, token->data, token->size);
}
void ident_get_expr(struct ast_parser *parser, struct node_st *expr) {
    parser_end return;
    struct token_st *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_Identifier) return;
    parser->position++;
    expr->type = PrimType_Ident_get;
    expr->main_type = MainType_Expr;
    expr->data = object_new();
    object_set_type(expr->data, STRING_TYPE);
    string_set_str(expr->data->data, token->data, token->size);
}
void bool_expr(struct ast_parser *parser, struct node_st *expr) {
    parser_end return;
    struct token_st *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_KeyWords || (token->subtype != KeyWord_FALSE && token->subtype != KeyWord_TRUE))
        return;
    parser->position++;
    expr->type = PrimType_Literal;
    expr->main_type = MainType_Expr;
    expr->data = object_new();
    object_set_type(expr->data, BOOL_TYPE);
    if (token->subtype == KeyWord_FALSE) ((struct bool_st *)expr->data->data)->data = 0;
    else ((struct bool_st *)expr->data->data)->data = 1;
}
void number_expr(struct ast_parser *parser, struct node_st *expr) {
    parser_end return;
    struct token_st *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_Int) return;
    parser->position++;
    expr->type = PrimType_Literal;
    expr->main_type = MainType_Expr;
    expr->data = object_new();
    object_set_type(expr->data, INTEGER_TYPE);
    if(token->subtype == IntType_bin)
        ((struct integer_st *)expr->data->data)->data = strtol(token->data, NULL, 2);
    else if(token->subtype == IntType_hex)
        ((struct integer_st *)expr->data->data)->data = strtol(token->data, NULL, 16);
    else if(token->subtype == IntType_oct)
        ((struct integer_st *)expr->data->data)->data = strtol(token->data, NULL, 8);
    else if(token->subtype == IntType_dec)
        ((struct integer_st *)expr->data->data)->data = strtol(token->data, NULL, 10);
}
void string_expr(struct ast_parser *parser, struct node_st *expr) {
    parser_end return;
    struct token_st *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_String) return;
    parser->position++;
    expr->type = PrimType_Literal;
    expr->main_type = MainType_Expr;
    expr->data = object_new();
    object_set_type(expr->data, STRING_TYPE);
    string_set_str(expr->data->data, token->data, token->size);
}
void null_expr(struct ast_parser *parser, struct node_st *expr) {
    parser_end return;
    struct token_st *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_KeyWords || token->subtype != KeyWord_EMPTY) return;
    parser->position++;
    expr->type = PrimType_Literal;
    expr->main_type = MainType_Expr;
    expr->data = object_new();
}
void literal_expr(struct ast_parser *parser, struct node_st *expr) {
    null_expr(parser, expr);
    if (expr->type != ExprType_None) return;
    string_expr(parser, expr);
    if (expr->type != ExprType_None) return;
    number_expr(parser, expr);
    if (expr->type != ExprType_None) return;
    bool_expr(parser, expr);
    if (expr->type != ExprType_None) return;
    list_expr(parser, expr);
}
void atom_expr(struct ast_parser *parser, struct node_st *expr) {
    ident_get_expr(parser, expr);
    if (expr->type != ExprType_None) return;
    literal_expr(parser, expr);
    if (expr->type != ExprType_None) return;
    scopes_expr(parser, expr);
}
void primary_expr(struct ast_parser *parser, struct node_st *expr) {
    size_t current_pointing = parser->position;
    struct node_st *expr_next = expr;
    struct token_st *token = NULL;
    int result = 0;
    {
        atom_expr(parser, expr_next);
        if (expr->type == ExprType_None) goto end;
        while(parser->position < parser->list->size){
            token = parser->list->data[parser->position]->data;
            if (token->type == TokenType_Special && token->subtype == Special_DOT) {
                parser->position++;

                expr_cast(expr)
                expr->type = PrimType_Attrib;
                expr->main_type = MainType_Expr;
                expr_add(expr)

                ident_get_expr(parser, expr_next);
                if (expr_next->type == ExprType_None) goto end;

                continue;
            } else if (token->type == TokenType_Special && token->subtype == Special_LSQB) {
                parser->position++;

                expr_cast(expr)
                expr->type = PrimType_Subscript;
                expr->main_type = MainType_Expr;
                expr_add(expr)

                or_test_oper(parser, expr_next);
                if (expr_next->type == ExprType_None) goto end;

                parser_end goto end;
                token = parser->list->data[parser->position]->data;
                if (token->type != TokenType_Special || token->subtype != Special_RSQB) goto end;
                parser->position++;

                continue;
            } else if (token->type == TokenType_Special && token->subtype == Special_LSB) {
                expr_cast(expr)
                expr->type = PrimType_Call;
                expr->main_type = MainType_Expr;
                expr_add(expr)

                list_oper(parser, expr_next, Special_LSB, Special_RSB);
                if (expr_next->type == ExprType_None) goto end;

                continue;
            }
            break;
        }
        result = 1;
    }
    end:
    if (!result) {
        node_clear(expr);
        parser->position = current_pointing;
    }
}