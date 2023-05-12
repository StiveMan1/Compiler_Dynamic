#include "basic.h"

struct object_type darray_type = {DARRAY_OP};
// Standard operations
// Create
struct darray_st *darray_new() {
    struct darray_st *res = Malloc(DARRAY_SIZE);
    res->data[0] = res->data[1] = NULL;
    res->size = res->max_size = 0;
    return res;
}
// Set value
void darray_set(struct darray_st *res, const struct darray_st *a) {
    darray_resize(res, 0);
    darray_resize(res, a->size);
    for (size_t i = 0; i < a->size; i++) {
        res->data[0][i] = object_copy(a->data[0][i]);
        res->data[1][i] = object_copy(a->data[1][i]);
    }
}
// Clear
void darray_clear(struct darray_st *res) {
    darray_resize(res, 0);
}
// Free
void darray_free(struct darray_st *res) {
    darray_resize(res, 0);
    if (res->max_size != 0) {
        Free(res->data[0]);
        Free(res->data[1]);
    }
    Free(res);
}
// Cmp
int darray_cmp(const struct darray_st *obj1, const struct darray_st *obj2) {
    if (obj1->size > obj2->size) return 1;
    if (obj1->size < obj2->size) return -1;
    int res_cmp_sub;
    struct error_st err;
    for (size_t i = 0; i < obj1->size; i++) {
        res_cmp_sub = object_cmp(&err, obj1->data[0][i], obj2->data[0][i]);
        if (res_cmp_sub == -1) return -1;
        if (res_cmp_sub == 1) return 1;
        res_cmp_sub = object_cmp(&err, obj1->data[1][i], obj2->data[1][i]);
        if (res_cmp_sub == -1) return -1;
        if (res_cmp_sub == 1) return 1;
    }
    return 0;
}
// Check for null
int darray_is_null(const struct darray_st *res) {
    return (res == NULL || res->size == 0);
}
// Change size of the darray
void darray_resize(struct darray_st *res, size_t size) {
    if (res->data[0] == NULL && size != 0) {
        res->max_size = size;
        res->data[0] = Malloc(POINTER_SIZE * size);
        res->data[1] = Malloc(POINTER_SIZE * size);
        for (size_t i = 0; i < size; i++) {
            res->data[0][i] = NULL;
            res->data[1][i] = NULL;
        }
    } else if (res->max_size < size) {
        res->data[0] = Realloc(res->data[0], POINTER_SIZE * size * 2);
        res->data[1] = Realloc(res->data[1], POINTER_SIZE * size * 2);
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
// Append the darray
void darray_append(struct darray_st *res, struct object_st *ptr, struct object_st *ref) {
    if (res == NULL || ptr == NULL) return;

    size_t pos = res->size;
    darray_resize(res, pos + 1);
    res->data[0][pos] = object_copy(ptr);
    res->data[1][pos] = object_copy(ref);
}