#include "basic.h"

struct object_type integer_type = {INTEGER_OP};
// Standard operations
struct integer_st *integer_new() {
    struct integer_st *res = malloc(INTEGER_SIZE);
    res->data = 0;
    return res;
}
void integer_set(struct integer_st *res, const struct integer_st *a) {
    if (integer_is_null(a)) return integer_clear(res);
    res->data = a->data;
}
void integer_clear(struct integer_st *res) {
    res->data = 0;
}
void integer_free(struct integer_st *res) {
    free(res);
}
int integer_cmp(const struct integer_st *obj1, const struct integer_st *obj2) {
    if(obj1->data < obj2->data) return -1;
    if(obj1->data > obj2->data) return 1;
    return 0;
}

// Cmp methods
int integer_is_null(const struct integer_st *res) {
    if (res == NULL || res->data == 0) return 1;
    return 0;
}

