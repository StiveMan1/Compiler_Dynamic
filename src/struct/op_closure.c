#include "struct.h"

struct object_type op_closure_type = {OP_CLOSURE_OP};

struct op_closure *op_closure_new(){
    struct op_closure *res = malloc(OP_CLOSURE_SIZE);
    res->data[0] = res->data[1] = NULL;
    res->size = res->max_size = 0;
    return res;
}
void op_closure_set(struct op_closure *res, const struct op_closure *a){
    op_closure_closure_resize(res, 0);
    op_closure_closure_resize(res, a->size);
    for (size_t i = 0; i < a->size; i++) {
        res->data[0][i] = object_copy(a->data[0][i]);
        res->data[1][i] = object_copy(a->data[1][i]);
    }
}
void op_closure_clear(struct op_closure *res){
    op_closure_closure_resize(res, 0);
}
void op_closure_free(struct op_closure *res){
    op_closure_closure_resize(res, 0);
    if(res->max_size != 0) {
        free(res->data[0]);
        free(res->data[1]);
    }
    free(res);
}

void op_closure_closure_resize(struct op_closure *res, size_t size) {
    if (res->data[0] == NULL && size != 0) {
        res->max_size = size;
        res->data[0] = malloc(POINTER_SIZE * size);
        res->data[1] = malloc(POINTER_SIZE * size);
        for (size_t i = 0; i < size; i++) {
            res->data[0][i] = NULL;
            res->data[1][i] = NULL;
        }
    } else if (res->max_size < size) {
        res->data[0] = realloc(res->data[0], POINTER_SIZE * size * 2);
        res->data[1] = realloc(res->data[1], POINTER_SIZE * size * 2);
        for (size_t i = res->max_size; i < size * 2; i++) {
            res->data[0][i] = NULL;
            res->data[1][i] = NULL;
        }
        res->max_size = size * 2;
    }
    if (res->size > size) {
        for (size_t i = size; i < res->size; i++) {
            if (res->data[0][i] != NULL) object_free(res->data[0][i]);
            if (res->data[1][i] != NULL) object_free(res->data[1][i]);
            res->data[0][i] = NULL;
            res->data[1][i] = NULL;
        }
    }
    res->size = size;
}
void op_closure_append(struct op_closure *res, struct object_st *ptr, struct object_st *ref) {
    if (res == NULL || ptr == NULL || ref == NULL) return;

    size_t pos = res->size;
    op_closure_closure_resize(res, pos + 1);
    res->data[0][pos] = object_copy(ptr);
    res->data[1][pos] = object_copy(ref);
}