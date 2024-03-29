#include "struct.h"

// Create
struct ast_parser *ast_parser_new() {
    struct ast_parser *res = Malloc(sizeof(struct ast_parser));
    res->position = 0;
    res->error_obj = error_new();
    res->list = array_new();
    res->tree = node_new();

    res->scope_type = ScopeType_None;

    res->memory_frame = array_new();
    res->memory_closure = array_new();
    res->expr_stack = array_new();
    return res;
}
// Setter
void ast_parser_set(struct ast_parser *res, const struct ast_parser *a) {
    res->position = a->position;
    error_set(res->error_obj, a->error_obj);
    array_set(res->list, a->list);
    node_set(res->tree, a->tree);
}
// Clear
void ast_parser_clear(struct ast_parser *res) {
    res->position = 0;
    error_clear(res->error_obj);
    array_clear(res->list);
    node_clear(res->tree);
}
// Free
void ast_parser_free(struct ast_parser *res) {
    array_free(res->list);
    error_free(res->error_obj);
    node_free(res->tree);

    array_free(res->memory_frame);
    array_free(res->memory_closure);
    array_free(res->expr_stack);
    Free(res);
}
// Set list from la_parser
void ast_parser_set_list(struct ast_parser *res, struct la_parser *parser) {
    res->position = 0;
    error_clear(res->error_obj);
    array_set(res->list, parser->list);
}
// Get error
void ast_parser_get_error(struct ast_parser *res, struct error_st *error) {
    error_set(error, res->error_obj);
}
// Save type
void ast_parser_save_type(struct ast_parser *res, struct node_st *obj) {
    array_add_new(res->expr_stack, INTEGER_TYPE);
    ((struct integer_st *)res->expr_stack->data[res->expr_stack->size - 1]->data)->data = res->scope_type;

    array_add_new(res->memory_frame, ARRAY_TYPE);
    array_append(res->memory_closure, obj->closure);
}
// Remove data frame from parser
void ast_parser_remove_frame(struct ast_parser *res) {
    array_remove_last(res->memory_frame);
    array_remove_last(res->memory_closure);
}

// Identifiers checking 
struct object_st *ast_parser_set_ident(struct ast_parser *res, struct object_st *obj) {
    struct array_st *array = array_get_last(res->memory_frame)->data;
    for (size_t i = 0; i < array->size; i++) {
        if(op_attrib_cmp_name(array->data[i]->data, obj->data) == 0)  {
            return object_copy(array->data[i]);
        }
    }
    array_add_new(array, OP_ATTRIB_TYPE);
    op_attrib_set_name(array_get_last(array)->data, obj->data);
    return object_copy(array_get_last(array));
}
struct object_st *ast_parser_get_ident(struct ast_parser *res, struct object_st *obj) {
    struct array_st *array;
    struct darray_st *closure;

    size_t i = res->memory_frame->size;
    struct object_st *ptr = NULL;

    for (; i > 0; i--) {
        array = res->memory_frame->data[i - 1]->data;
        for (size_t j = 0; j < array->size; j++) {
            if(op_attrib_cmp_name(array->data[j]->data, obj->data) == 0) {
                ptr = object_copy(array->data[j]);
                break;
            }
        }
        if(ptr != NULL) break;
    }

    if(ptr == NULL) return NULL;

    for (; i < res->memory_frame->size; i++){
        if(res->memory_closure->data[i] == NULL) continue;
        array = res->memory_frame->data[i]->data;
        closure = res->memory_closure->data[i]->data;

        struct object_st *new_obj = object_new();
        object_set_type(new_obj, OP_ATTRIB_TYPE);
        op_attrib_set_name(new_obj->data, obj->data);

        darray_append(closure, new_obj, ptr);
        array_append(array, new_obj);

        object_free(ptr);
        ptr = new_obj;
    }

    return ptr;
}