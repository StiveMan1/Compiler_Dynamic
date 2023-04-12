#include "basic.h"


struct object_type object_type = {OBJECT_OP};
// Standard operations
struct object_st *object_new() {
    struct object_st *res = malloc(OBJECT_SIZE);
    res->type = NULL;
    res->data = NULL;
    res->counter = 1;
    return res;
}
void object_set(struct object_st *res, const struct object_st *a) {
    if (res == NULL || a == NULL) return;
    object_set_type(res, a->type);
    if (res->type != NULL && res->type->self._set != NULL) res->type->self._set(res->data, (a)->data);
}
void object_clear(struct object_st *res) {
    if (res == NULL) return;
    object_set_type(res, NONE_TYPE);
}
void object_free(struct object_st *res) {
    if (res == NULL || --res->counter > 0) return;
    if (res->data != NULL) {
        if (res->type != NULL && res->type->self._free != NULL)
            res->type->self._free(res->data);
        res->data = NULL;
    }
    free(res);
}
int object_cmp(const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;

    if (obj1 == obj2) return 0;
    if (obj1 == NULL || obj2 == NULL) return 2;
    if (obj1->type != obj2->type) return 2;
    if (obj1->type != NULL && obj1->type->self._cmp != NULL) return obj1->type->self._cmp(obj1->data, obj2->data);
    return 0;
}

// Class methods
struct object_st *object_copy(struct object_st *res) {
    if (res == NULL) return NULL;
    res->counter++;
    return res;
}
void object_set_type(struct object_st *res, struct object_type *type) {
    if (res == NULL) return;
    if (res->type == type) return;
    if (res->type != NULL) {
        if (res->data != NULL) {
            if (res->type != NULL && res->type->self._free != NULL)
                res->type->self._free(res->data);
            res->data = NULL;
        }
    }
    res->type = type;
    if (res->type != NULL && res->type->self._new != NULL) res->data = res->type->self._new();
}