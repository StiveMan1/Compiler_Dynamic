#include "ast_analyze.h"

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

int type_expr(struct ast_parser *parser, struct node_st *expr) {
    int result = type_int(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = type_real(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = type_bool(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = type_string(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = type_array(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = type_tuple(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = type_func(parser, expr);
    return result;
}
// Integer type
int type_int(struct ast_parser *parser, struct node_st *expr) {
    parser_end return SN_Status_EOF;
    struct token_st *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_KeyWords || token->subtype != KeyWord_INT) return SN_Status_Nothing;
    parser->position++;
    expr->type = Type_int;
    expr->main_type = MainType_Type;
    node_set_position(expr, parser->list->data[parser->position - 1]->data);
    return SN_Status_Success;
}
// Real type
int type_real(struct ast_parser *parser, struct node_st *expr) {
    parser_end return SN_Status_EOF;
    struct token_st *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_KeyWords || token->subtype != KeyWord_REAL) return SN_Status_Nothing;
    parser->position++;
    expr->type = Type_real;
    expr->main_type = MainType_Type;
    node_set_position(expr, parser->list->data[parser->position - 1]->data);
    return SN_Status_Success;
}
// Bool type
int type_bool(struct ast_parser *parser, struct node_st *expr) {
    parser_end return SN_Status_EOF;
    struct token_st *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_KeyWords || token->subtype != KeyWord_BOOL) return SN_Status_Nothing;
    parser->position++;
    expr->type = Type_bool;
    expr->main_type = MainType_Type;
    node_set_position(expr, parser->list->data[parser->position - 1]->data);
    return SN_Status_Success;
}
// String type
int type_string(struct ast_parser *parser, struct node_st *expr) {
    parser_end return SN_Status_EOF;
    struct token_st *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_KeyWords || token->subtype != KeyWord_STRING) return SN_Status_Nothing;
    parser->position++;
    expr->type = Type_string;
    expr->main_type = MainType_Type;
    node_set_position(expr, parser->list->data[parser->position - 1]->data);
    return SN_Status_Success;
}
// Array type 
int type_array(struct ast_parser *parser, struct node_st *expr) {
    parser_end return SN_Status_EOF;
    struct token_st *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_Special || token->subtype != Special_LSQB) return SN_Status_Nothing;
    parser->position++;
    token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_Special || token->subtype != Special_RSQB) return SN_Status_Nothing;
    parser->position++;
    expr->type = Type_array;
    expr->main_type = MainType_Type;
    node_set_position(expr, parser->list->data[parser->position - 1]->data);
    return SN_Status_Success;
}
// Tuple type
int type_tuple(struct ast_parser *parser, struct node_st *expr) {
    parser_end return SN_Status_EOF;
    struct token_st *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_Special || token->subtype != Special_LCB) return SN_Status_Nothing;
    parser->position++;
    token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_Special || token->subtype != Special_RCB) return SN_Status_Nothing;
    parser->position++;
    expr->type = Type_tuple;
    expr->main_type = MainType_Type;
    node_set_position(expr, parser->list->data[parser->position - 1]->data);
    return SN_Status_Success;
}
// Function type
int type_func(struct ast_parser *parser, struct node_st *expr) {
    parser_end return SN_Status_EOF;
    struct token_st *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_KeyWords || token->subtype != KeyWord_FUNC) return SN_Status_Nothing;
    parser->position++;
    expr->type = Type_func;
    expr->main_type = MainType_Type;
    node_set_position(expr, parser->list->data[parser->position - 1]->data);
    return SN_Status_Success;
}