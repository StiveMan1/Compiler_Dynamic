#include <stdio.h>
#include "ast_semantic.h"



struct semantic_state *semantic_state_new() {
    struct semantic_state *res = malloc(sizeof(struct semantic_state));
    res->memory_stack = array_new();
    res->memory_size_stack = array_new();

    return res;
}

void semantic_state_free(struct semantic_state *res) {
    array_free(res->memory_stack);
    array_free(res->memory_size_stack);
    free(res);
}

void semantic_state_add(struct semantic_state *res, struct object_st *obj) {
    array_append(res->memory_stack, obj);
}

int semantic_state_find(struct semantic_state *res, struct object_st *obj) {
    for (size_t i = 0; i < res->memory_stack->size; i++) {
        if (object_cmp(obj, res->memory_stack->data[i]) == 0) return 1;
    }
    return 0;
}

void semantic_state_append_scope(struct semantic_state *res){
    struct object_st * obj = object_new();
    object_set_type(obj, INTEGER_TYPE);
    ((struct integer_st *)obj->data)->data = (long int)res->memory_stack->size;
    array_append(res->memory_size_stack, obj);
    object_free(obj);
}

void semantic_state_remove_scope(struct semantic_state *res){
    struct object_st * obj = res->memory_size_stack->data[res->memory_size_stack->size - 1];
    array_resize(res->memory_stack, ((struct integer_st *)obj->data)->data);
    array_resize(res->memory_size_stack, res->memory_size_stack->size - 1);
}

void semantic_check(struct semantic_state *state, struct object_st *obj) {
    if (obj->type != NODE_TYPE) return;
    struct node_st *node = obj->data;


    if (node->main_type == MainType_Expr) {
        switch (node->type) {
            case PrimType_Ident_new:
                semantic_state_add(state, node->data);
                return;
            case PrimType_Ident_get:
                if(!semantic_state_find(state,node->data)){
                    printf("Semantic  Error\n");
                    exit(-1);
                }
                return;
        }
    }
    if (node->main_type == MainType_Stmt && node->type == StmtType_List) semantic_state_append_scope(state);

    if (!array_is_null(node->next)) {
        for (size_t i = 0; i < node->next->size; i++)
            semantic_check(state, node->next->data[i]);
    }
    if (node->main_type == MainType_Stmt && node->type == StmtType_List) semantic_state_remove_scope(state);
}