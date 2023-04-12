#include "basic.h"

struct object_type string_type = {STRING_OP};
// Standard operations
struct string_st *string_new() {
    struct string_st *res = malloc(STRING_SIZE);
    res->data = NULL;
    res->mx_size = res->size = 0;
    return res;
}
void string_set(struct string_st *res, const struct string_st *a) {
    if (string_is_null(a)) return string_clear(res);
    string_resize(res, a->size);
    memcpy(res->data, a->data, a->size);
}
void string_clear(struct string_st *res) {
    string_resize(res, 0);
}
void string_free(struct string_st *res) {
    if (res->data != NULL) free(res->data);
    free(res);
}
int string_cmp(const struct string_st *obj1, const struct string_st *obj2) {
    if (obj1->size > obj2->size) return 1;
    if (obj1->size < obj2->size) return -1;
    return memcmp(obj1->data, obj2->data, obj1->size);
}
int string_is_null(const struct string_st *res){
    return (res == NULL || res->size == 0);
}

// Class methods
void string_resize(struct string_st *res, size_t size) {
    if (res->data == NULL && size != 0) {
        res->mx_size = size;
        res->data = malloc(size + 1);
        if (res->data != NULL) for (size_t i = 0; i < size + 1; i++) res->data[i] = 0;
    } else if (res->mx_size < size) {
        res->data = realloc(res->data, size * 2 + 1);
        if (res->data != NULL) for (size_t i = res->mx_size; i < size * 2 + 1; i++) res->data[i] = 0;
        res->mx_size = size * 2;
    }
    if (res->size > size)
        for (size_t i = size; i < res->size; i++) {
            res->data[i] = 0;
        }
    res->size = size;
}
void string_set_str(struct string_st *res, const char *str, size_t size) {
    if (res == NULL) return;
    if (str == NULL) return string_clear(res);

    string_resize(res, size);
    memcpy(res->data, str, size);
}
void string_concat(struct string_st *res, const struct string_st *a) {
    if (res == NULL || string_is_null(a)) return;

    size_t _size = res->size;
    string_resize(res, res->size + a->size);
    memcpy(res->data + _size, a->data, a->size);
}
