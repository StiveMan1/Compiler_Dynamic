#include "ast_semantic.h"


void semantic_scan_fields(struct ast_parser *parser, struct object_st *obj) {
    struct node_st *node = obj->data;

    if (node->main_type == MainType_Expr) {
        switch (node->type) {
            case PrimType_Ident_get: {
                struct object_st *res = ast_parser_get_ident(parser, node->data);
                if (res == NULL) {
                    error_fill_in(parser->error_obj, SEMANTIC_ANALYSIS_ERROR, "Identifier not initialized", node->pos, node->line_num, node->line_pos);
                    return;
                }
                object_free(node->data);
                node->data = res;
                return;
            }
            case PrimType_Ident_new: {
                struct object_st *res = ast_parser_set_ident(parser, node->data);
                object_free(node->data);
                node->data = res;
                return;
            }
        }
    }
    if (node->main_type == MainType_Stmt) {
        switch (node->type) {
            case StmtType_Params:
            case StmtType_Return:
                if (((parser->type) & ScopeType_Func) != ScopeType_Func) {
                    error_fill_in(parser->error_obj, SEMANTIC_ANALYSIS_ERROR, "Return Statement not in Function Scopes", node->pos, node->line_num, node->line_pos);
                    return;
                }
                break;
            case StmtType_Func_Body:
                ast_parser_save_type(parser, node);
                node->closure = object_new();
                object_set_type(node->closure, DARRAY_TYPE);
                ast_parser_save_type(parser, node);
                parser->type = ScopeType_Func;
                break;
            case StmtType_For:
            case StmtType_While:
                ast_parser_save_type(parser, node);
                parser->type |= ScopeType_Loop;
                break;
            case StmtType_List:
                ast_parser_save_type(parser, node);
                break;
        }
    }

    for (size_t i = 0; i < node->next->size; i++) {
        array_append(parser->expr_stack, node->next->data[node->next->size - i - 1]);
    }

}

void semantic_scan(struct ast_parser *parser) {
    struct object_st *obj = object_new();
    object_set_type(obj, NODE_TYPE);
    node_set(obj->data, parser->tree);

    array_append(parser->expr_stack, obj);
    object_free(obj);
    while (parser->expr_stack->size && !parser->error_obj->present) {
        obj = object_copy(array_get_last(parser->expr_stack));
        array_remove_last(parser->expr_stack);

        if (obj->type == NODE_TYPE) {
            semantic_scan_fields(parser, obj);
        } else if (obj->type == INTEGER_TYPE) {
            parser->type = ((struct integer_st *)obj->data)->data;
            ast_parser_remove_frame(parser);
        }

        object_free(obj);
    }
}