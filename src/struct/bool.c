#include "struct.h"

struct object_type bool_type = {BOOL_OP};
// Standard operations
struct bool_st *bool_new() {
    struct bool_st *res = malloc(BOOL_SIZE);
    res->data = 0;
    return res;
}
void bool_set(struct bool_st *res, const struct bool_st *a) {
    if (bool_is_null(a)) return bool_clear(res);
    res->data = a->data;
}
void bool_clear(struct bool_st *res) {
    res->data = 0;
}
void bool_free(struct bool_st *res) {
    free(res);
}
int bool_cmp(const struct bool_st *obj1, const struct bool_st *obj2) {
    if(obj1->data < obj2->data) return -1;
    if(obj1->data > obj2->data) return 1;
    return 0;
}


// Cmp methods
int bool_is_null(const struct bool_st *res) {
    if (res == NULL || res->data == 0) return 1;
    return 0;
}

