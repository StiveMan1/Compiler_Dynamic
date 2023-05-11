#include "basic.h"

struct object_type func_type = {FUNC_OP};

struct func_st *func_new() {
    struct func_st *res = Malloc(FUNC_SIZE);
    res->data = NULL;

    res->class = NULL;
    res->attr = map_new();

    res->closure = darray_new();

    return res;
}
void func_set(struct func_st *res, const struct func_st *a) {
    func_clear(res);

    res->data = object_copy(a->data);

    res->class = object_copy(a->class);
    map_set(res->attr, a->attr);

    darray_set(res->closure, a->closure);
}
void func_clear(struct func_st *res) {
    if(res->data != NULL) object_free(res->data);
    res->data = NULL;

    if(res->class != NULL) object_free(res->class);
    res->class = NULL;
    map_clear(res->attr);

    darray_clear(res->closure);
}
void func_free(struct func_st *res) {
    if(res->data != NULL) object_free(res->data);

    if(res->class != NULL) object_free(res->class);
    map_free(res->attr);

    darray_free(res->closure);
    Free(res);
}

void func_set_function(struct func_st *res, struct node_st *node) {
    func_clear(res);

    struct object_st *obj = NULL;
    struct string_st *attr_name = string_new();

    string_set_str(attr_name, "__params__", 10);
    obj = map_set_elm(res->attr, attr_name);
    object_set(obj, node->next->data[0]);
    object_free(obj);


    string_set_str(attr_name, "__call__", 7);
    obj = map_set_elm(res->attr, attr_name);
    object_set(obj, node->next->data[1]);
    object_free(obj);

    string_set_str(attr_name, "__closure__", 11);
    obj = map_set_elm(res->attr, attr_name);
    object_set(obj, node->closure);
    if (obj->data != NULL) {
        struct darray_st *closure = obj->data;
        struct object_st *temp = NULL;
        for (size_t i = 0 ; i < closure->size; i++) {
            temp = closure->data[1][i];
            closure->data[1][i] = object_copy(((struct op_attrib *)temp->data)->data);
            object_free(temp);
        }
    }
    object_free(obj);

    string_free(attr_name);
}
struct object_st *func_get_attrib(const struct func_st *res, struct string_st *name) {
    return map_get_elm(res->attr, name);
}
struct object_st *func_set_attrib(const struct func_st *res, const struct string_st *name) {
    return map_set_elm(res->attr, name);
}