#include "struct.h"
#include <sys/time.h>

struct object_type real_type = {REAL_OP};
// Standard operations
struct real_st *real_new() {
    struct real_st *res = malloc(REAL_SIZE);
    res->data = 0;
    return res;
}
void real_set(struct real_st *res, const struct real_st *a) {
    if (real_is_null(a)) return real_clear(res);
    res->data = a->data;
}
void real_clear(struct real_st *res) {
    res->data = 0;
}
void real_free(struct real_st *res) {
    free(res);
}
int real_cmp(const struct real_st *obj1, const struct real_st *obj2) {
    if(obj1->data < obj2->data) return -1;
    if(obj1->data > obj2->data) return 1;
    return 0;
}


// Cmp methods
int real_is_null(const struct real_st *res) {
    if (res == NULL || res->data == 0) return 1;
    return 0;
}

