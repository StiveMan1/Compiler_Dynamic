#include "struct.h"

struct object_type array_type = {ARRAY_OP};
// Standard operations
struct array_st *array_new() {
    struct array_st *res = malloc(ARRAY_SIZE);
    res->data = NULL;
    res->max_size = res->size = 0;
    return res;
}
void array_set(struct array_st *res, const struct array_st *a) {
    if (a == NULL) return;
    array_resize(res, 0);
    array_resize(res, a->size);
    for (size_t i = 0; i < a->size; i++) res->data[i] = object_copy(a->data[i]);
}
void array_clear(struct array_st *res) {
    array_resize(res, 0);
}
void array_free(struct array_st *res) {
    array_resize(res, 0);
    if (res->data != NULL) free(res->data);
    free(res);
}
int array_cmp(const struct array_st *obj1, const struct array_st *obj2) {
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
int array_is_null(const struct array_st *res) {
    return (res == NULL || res->size == 0);
}

// Class methods
void array_resize(struct array_st *res, size_t size) {
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
void array_append(struct array_st *res, struct object_st *obj) {
    if (res == NULL) return;

    array_resize(res, res->size + 1);
    res->data[res->size - 1] = object_copy(obj);
}
void array_concat(struct array_st *res, const struct array_st *a) {
    if (res == NULL || array_is_null(a)) return;

    size_t _size = res->size;
    array_resize(res, res->size + a->size);
    for (size_t i = 0; i < a->size; i++) {
        res->data[_size + i] = object_copy(a->data[i]);
    }
}
void array_add_new(struct array_st *res, struct object_type *type) {
    if (res == NULL) return;

    array_resize(res, res->size + 1);
    res->data[res->size - 1] = object_new();
    object_set_type(res->data[res->size - 1], type);
}
void array_remove_last(struct array_st *res) {
    if(res->size <= 0) return;
    array_resize(res, res->size - 1);
}
struct object_st *array_get_last(struct array_st *res) {
    if(res->size <= 0) return NULL;
    return res->data[res->size - 1];
}


void array_sort_merge(size_t st1, size_t fn1, size_t st2, size_t fn2, struct object_st **data, struct object_st **temp) {
    size_t st = st1;
    size_t pos = st1;
    while (st1 < fn1 || st2 < fn2) {
        if (st1 == fn1) {
            temp[pos++] = data[st2++];
        } else if (st2 == fn2) {
            temp[pos++] = data[st1++];
        } else {
            if (object_cmp(data[st1], data[st2]) <= 0) {
                temp[pos++] = data[st1++];
            } else {
                temp[pos++] = data[st2++];
            }
        }
    }
    for (; st < fn2; st++) {
        data[st] = temp[st];
        temp[st] = NULL;
    }
}
void array_sort_split(size_t st, size_t fn, struct object_st **data, struct object_st **temp) {
    if (st + 1 >= fn) return;
    size_t mid = (st + fn) / 2;
    array_sort_split(st, mid, data, temp);
    array_sort_split(mid, fn, data, temp);
    array_sort_merge(st, mid, mid, fn, data, temp);
}
void array_sort(struct array_st *res) {
    if(res == NULL) return;

    struct array_st *temp = array_new();
    array_resize(temp, res->size);
    array_sort_split(0, res->size, res->data, temp->data);
    array_free(temp);
}
