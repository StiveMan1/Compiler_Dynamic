#include "ast_semantic.h"


struct sm_state *sm_state_new() {
    struct sm_state *res = malloc(sizeof(struct sm_state));
    res->type = ScopeType_None;
    res->error = 0;

    res->memory_frame = array_new();
    res->memory_closure = array_new();
    res->expr_stack = array_new();
    return res;
}
void sm_state_free(struct sm_state *res) {
    array_free(res->memory_frame);
    array_free(res->memory_closure);
    array_free(res->expr_stack);
    free(res);
}
void sm_state_save_type(struct sm_state *res, struct node_st *obj) {
    array_add_new(res->expr_stack, INTEGER_TYPE);
    ((struct integer_st *)res->expr_stack->data[res->expr_stack->size - 1]->data)->data = res->type;

    array_add_new(res->memory_frame, ARRAY_TYPE);
    array_append(res->memory_closure, obj->closure);
}
void sm_state_remove_frame(struct sm_state *res) {
    array_remove_last(res->memory_frame);
    array_remove_last(res->memory_closure);
}

struct object_st *sm_state_set_ident(struct sm_state *res, struct object_st *obj) {
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
struct object_st *sm_state_get_ident(struct sm_state *res, struct object_st *obj) {
    struct array_st *array;
    struct op_closure *closure;

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

        op_closure_append(closure, new_obj, ptr);
        array_append(array, new_obj);

        object_free(ptr);
        ptr = new_obj;
    }

    return ptr;
}

void semantic_scan_fields(struct sm_state *state, struct object_st *obj) {
    struct node_st *node = obj->data;

    if (node->main_type == MainType_Expr) {
        switch (node->type) {
            case PrimType_Ident_get: {
                struct object_st *res = sm_state_get_ident(state, node->data);
                if (res == NULL) {
                    state->error = -SemanticError_Ident;
                    return;
                }
                object_free(node->data);
                node->data = res;
                return;
            }
            case PrimType_Ident_new: {
                struct object_st *res = sm_state_set_ident(state, node->data);
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
                if (((state->type) & ScopeType_Func) != ScopeType_Func) {
                    state->error = -SemanticError_Return;
                    return;
                }
                break;
            case StmtType_Func_Body:
                sm_state_save_type(state, node);

                state->type = ScopeType_Func;
                break;
            case StmtType_For:
            case StmtType_While:
                sm_state_save_type(state, node);
                state->type |= ScopeType_Loop;
                break;
            case StmtType_List:
                sm_state_save_type(state, node);
                break;
        }
    }

    for (size_t i = 0; i < node->next->size; i++) {
        array_append(state->expr_stack, node->next->data[node->next->size - i - 1]);
    }

}

int semantic_scan(struct object_st *expr_obj) {
    struct sm_state *state = sm_state_new();
    struct object_st *obj;

    array_append(state->expr_stack, expr_obj);
    while (state->expr_stack->size && state->error == 0) {
        obj = object_copy(array_get_last(state->expr_stack));
        array_remove_last(state->expr_stack);

        if (obj->type == NODE_TYPE) {
            semantic_scan_fields(state, obj);
        } else if (obj->type == INTEGER_TYPE) {
            state->type = ((struct integer_st *)obj->data)->data;
            sm_state_remove_frame(state);
        }

        object_free(obj);
    }
    sm_state_free(state);
    return state->error;
}