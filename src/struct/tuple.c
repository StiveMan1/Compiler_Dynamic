#include "struct.h"

struct object_type tuple_type = {TUPLE_OP};
// Standard operations
struct tuple_st *tuple_new() {
    struct tuple_st *res = malloc(TUPLE_SIZE);
    res->data = NULL;
    res->max_size = res->size = 0;
    return res;
}
void tuple_set(struct tuple_st *res, const struct tuple_st *a) {
    if (a == NULL) return;
    tuple_resize(res, 0);
    tuple_resize(res, a->size);
    for (size_t i = 0; i < a->size; i++) res->data[i] = object_copy(a->data[i]);
}
void tuple_clear(struct tuple_st *res) {
    tuple_resize(res, 0);
}
void tuple_free(struct tuple_st *res) {
    tuple_resize(res, 0);
    if (res->data != NULL) free(res->data);
    free(res);
}
int tuple_cmp(const struct tuple_st *obj1, const struct tuple_st *obj2) {
    if (obj1->size > obj2->size) return 1;
    if (obj1->size < obj2->size) return -1;
    int res_cmp_sub;
    for (size_t i = 0; i < obj1->size; i++) {
        res_cmp_sub = object_cmp(obj1->data[i], obj2->data[i]);
        if (res_cmp_sub == -1) return -1;
        if (res_cmp_sub == 1) return 1;
    }
    return 0;
}
int tuple_is_null(const struct tuple_st *res) {
    return (res == NULL || res->size == 0);
}

// Class methods
void tuple_resize(struct tuple_st *res, size_t size) {
    if (res->data == NULL && size != 0) {
        res->max_size = size;
        res->data = malloc(POINTER_SIZE * size);
        for (size_t i = 0; i < size; i++) res->data[i] = NULL;
    } else if (res->max_size < size) {
        res->data = realloc(res->data, POINTER_SIZE * size * 2);
        for (size_t i = res->max_size; i < size * 2; i++) res->data[i] = NULL;
        res->max_size = size * 2;
    }
    if (res->size > size) {
        for (size_t i = size; i < res->size; i++) {
            if (res->data[i] != NULL) object_free(res->data[i]);
            res->data[i] = NULL;
        }
    }
    res->size = size;
}
