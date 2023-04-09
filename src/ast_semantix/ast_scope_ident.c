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
void sm_state_save_type(struct sm_state *res, struct object_st *obj) {
    array_add_new(res->expr_stack, INTEGER_TYPE);
    ((struct integer_st *)res->expr_stack->data[res->expr_stack->size - 1]->data)->data = res->type;

    array_add_new(res->memory_frame, ARRAY_TYPE);
    array_append(res->memory_closure, obj);
}
void sm_state_remove_frame(struct sm_state *res) {
    array_resize(res->memory_frame, res->memory_frame->size - 1);
    array_resize(res->memory_closure, res->memory_closure->size - 1);
}

struct object_st *sm_state_set_ident(struct sm_state *res, struct object_st *obj) {
    struct array_st *list = res->memory_frame->data[res->memory_frame->size - 1]->data;
    for (size_t i = 0; i < list->size; i++) {
        if (object_cmp(list->data[i], obj) == 0) {
            return object_copy(list->data[i]);
        }
    }
    array_append(list, obj);
    return object_copy(obj);
}
struct object_st *sm_state_get_ident(struct sm_state *res, struct object_st *obj) {
    struct array_st *list;
    struct node_st *node;

    size_t i = res->memory_frame->size;
    struct object_st *ptr = NULL;

    for (; i > 0; i--) {
        list = res->memory_frame->data[i - 1]->data;
        for (size_t j = 0; j < list->size; j++) {
            if (object_cmp(list->data[j], obj) == 0) {
                ptr = list->data[j];
                break;
            }
        }
        if(ptr != NULL) break;
    }

    if(ptr == NULL) return NULL;

    for (; i < res->memory_frame->size; i++) {
        list = res->memory_frame->data[i]->data;
        node = res->memory_closure->data[i]->data;

        struct object_st *new_obj = object_new();
        object_set(new_obj, obj);
        node_closure_append(node, new_obj, ptr);
        array_append(list, new_obj);
        object_free(new_obj);

        ptr = new_obj;
    }

    return object_copy(ptr);
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
            case StmtType_Func:
                sm_state_save_type(state, obj);

                state->type = ScopeType_Func;
                break;
            case StmtType_For:
            case StmtType_While:
                sm_state_save_type(state, obj);
                state->type |= ScopeType_Loop;
                break;
            case StmtType_List:
                sm_state_save_type(state, obj);
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
    while (state->expr_stack->size) {
        obj = object_copy(state->expr_stack->data[state->expr_stack->size - 1]);
        array_resize(state->expr_stack, state->expr_stack->size - 1);

        if (obj->type == NODE_TYPE) {
            semantic_scan_fields(state, obj);
            if (state->error != 0) return state->error;
        } else if (obj->type == INTEGER_TYPE) {
            state->type = ((struct integer_st *)obj->data)->data;
            sm_state_remove_frame(state);
        }

        object_free(obj);
    }
    sm_state_free(state);
    return 0;
}