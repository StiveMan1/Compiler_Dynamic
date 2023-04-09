#include "struct.h"
#include "struct/ast_node.h"

struct object_type node_type = {NODE_OP};

struct node_st *node_new(){
    struct node_st *res = malloc(NODE_SIZE);
    res->main_type = MainType_None;
    res->type = ExprType_None;

    res->data = NULL;
    res->closure[0] = NULL;
    res->closure[1] = NULL;
    res->size = res->max_size = 0;

    res->next = array_new();
    res->tokens = array_new();
    return res;
}
void node_set(struct node_st *res, const struct node_st *a){
    res->main_type = a->main_type;
    res->type = a->type;
    if(res->data != NULL) object_free(res->data);
    res->data = object_copy(a->data);

    node_closure_resize(res, 0);
    node_closure_resize(res, a->size);
    for (size_t i = 0; i < a->size; i++) {
        res->closure[0][i] = object_copy(a->closure[0][i]);
        res->closure[1][i] = object_copy(a->closure[1][i]);
    }

    array_set(res->next, a->next);
    array_set(res->tokens, a->tokens);
}
void node_clear(struct node_st *res){
    res->main_type = MainType_None;
    res->type = ExprType_None;
    if(res->data != NULL) object_free(res->data);
    res->data = NULL;
    node_closure_resize(res, 0);

    array_clear(res->next);
    array_clear(res->tokens);
}
void node_free(struct node_st *res){
    if(res->data != NULL) object_free(res->data);

    node_closure_resize(res, 0);
    if(res->closure[0] != NULL) free(res->closure[0]);
    if(res->closure[1] != NULL) free(res->closure[1]);

    array_free(res->next);
    array_free(res->tokens);
    free(res);
}


void node_closure_resize(struct node_st *res, size_t size) {
    if (res->closure[0] == NULL && size != 0) {
        res->max_size = size;
        res->closure[0] = malloc(POINTER_SIZE * size);
        res->closure[1] = malloc(POINTER_SIZE * size);
        for (size_t i = 0; i < size; i++) {
            res->closure[0][i] = NULL;
            res->closure[1][i] = NULL;
        }
    } else if (res->max_size < size) {
        res->closure[0] = realloc(res->closure[0], POINTER_SIZE * size * 2);
        res->closure[1] = realloc(res->closure[1], POINTER_SIZE * size * 2);
        for (size_t i = res->max_size; i < size * 2; i++) {
            res->closure[0][i] = NULL;
            res->closure[1][i] = NULL;
        }
        res->max_size = size * 2;
    }
    if (res->size > size) {
        for (size_t i = size; i < res->size; i++) {
            if (res->closure[0][i] != NULL) object_free(res->closure[0][i]);
            if (res->closure[1][i] != NULL) object_free(res->closure[1][i]);
            res->closure[0][i] = NULL;
            res->closure[1][i] = NULL;
        }
    }
    res->size = size;
}
void node_closure_append(struct node_st *res, struct object_st *ptr, struct object_st *ref) {
    if (res == NULL || ptr == NULL || ref == NULL) return;

    node_closure_resize(res, res->size + 1);
    res->closure[0][res->size - 1] = object_copy(ptr);
    res->closure[1][res->size - 1] = object_copy(ref);
}