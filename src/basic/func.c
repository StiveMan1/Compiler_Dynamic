#include "basic.h"

struct object_type func_type = {FUNC_OP};

struct func_st *func_new() {
    struct func_st *res = Malloc(FUNC_SIZE);
    res->params = object_new();
    res->closure = object_new();
    res->body = object_new();
    return res;
}
void func_set(struct func_st *res, const struct func_st *a) {
    func_clear(res);
    object_set(res->params, a->params);
    object_set(res->closure, a->closure);
    object_set(res->body, a->body);
}
void func_clear(struct func_st *res) {
    object_clear(res->params);
    object_clear(res->closure);
    object_clear(res->body);
}
void func_free(struct func_st *res) {
    object_free(res->params);
    object_free(res->closure);
    object_free(res->body);
    Free(res);
}

void func_set_function(struct func_st *res, struct node_st *node) {
    func_clear(res);

    object_set(res->params, node->next->data[0]);
    object_set(res->body, node->next->data[1]);
    object_set(res->closure, node->closure);
    struct darray_st *closure = res->closure->data;
    struct object_st *temp = NULL;
    for (size_t i = 0 ; i < closure->size; i++) {
        temp = closure->data[1][i];
        closure->data[1][i] = object_copy(((struct op_attrib *)temp->data)->data);
        object_free(temp);
    }
}