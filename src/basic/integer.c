#include "basic.h"

struct object_math_op integer_math = {METHOD_MATH &integer__mod, METHOD_MATH &integer__and, METHOD_MATH &integer__mul, METHOD_MATH &integer__add, METHOD_MATH &integer__sub, METHOD_MATH &integer__div, METHOD_MATH &integer__xor, METHOD_MATH &integer__or, METHOD_MATH &integer__ls, METHOD_MATH &integer__rs, METHOD_CONVERT &integer__neg};
struct object_convert integer_convert = {METHOD_CONVERT &integer__bool, METHOD_CONVERT &integer__int, METHOD_CONVERT &integer__float, METHOD_CONVERT &integer__str};
struct object_type integer_type = {INTEGER_OP, &integer_math, &integer_convert};
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

// Math Methods
void integer__mod(struct object_st *res, const struct integer_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, INTEGER_TYPE);
    ((struct integer_st *)res->data)->data = obj1->data % ((struct integer_st *)obj2->data)->data;
}
void integer__and(struct object_st *res, const struct integer_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, INTEGER_TYPE);
    ((struct integer_st *)res->data)->data = (obj1->data & ((struct integer_st *)obj2->data)->data);
}
void integer__mul(struct object_st *res, const struct integer_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, INTEGER_TYPE);
    ((struct integer_st *)res->data)->data = obj1->data * ((struct integer_st *)obj2->data)->data;
}
void integer__add(struct object_st *res, const struct integer_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, INTEGER_TYPE);
    ((struct integer_st *)res->data)->data = obj1->data + ((struct integer_st *)obj2->data)->data;
}
void integer__sub(struct object_st *res, const struct integer_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, INTEGER_TYPE);
    ((struct integer_st *)res->data)->data = obj1->data - ((struct integer_st *)obj2->data)->data;
}
void integer__div(struct object_st *res, const struct integer_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, INTEGER_TYPE);
    ((struct integer_st *)res->data)->data = obj1->data / ((struct integer_st *)obj2->data)->data;
}
void integer__xor(struct object_st *res, const struct integer_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, INTEGER_TYPE);
    ((struct integer_st *)res->data)->data = (obj1->data ^ ((struct integer_st *)obj2->data)->data);
}
void integer__or(struct object_st *res, const struct integer_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, INTEGER_TYPE);
    ((struct integer_st *)res->data)->data = (obj1->data | ((struct integer_st *)obj2->data)->data);
}
void integer__ls(struct object_st *res, const struct integer_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, INTEGER_TYPE);
    ((struct integer_st *)res->data)->data = (obj1->data << ((struct integer_st *)obj2->data)->data);
}
void integer__rs(struct object_st *res, const struct integer_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != INTEGER_TYPE) return;
    object_set_type(res, INTEGER_TYPE);
    ((struct integer_st *)res->data)->data = (obj1->data >> ((struct integer_st *)obj2->data)->data);
}
void integer__neg(struct object_st *res, const struct integer_st *obj1) {
    object_set_type(res, INTEGER_TYPE);
    ((struct integer_st *)res->data)->data = -obj1->data;
}

// Convert Methods
void integer__bool(struct object_st *res, struct integer_st *obj){
    object_set_type(res, BOOL_TYPE);
    ((struct bool_st *)res->data)->data = !integer_is_null(obj);
}
void integer__int(struct object_st *res, struct integer_st *obj){
    object_set_type(res, INTEGER_TYPE);
    integer_set(res->data, obj);
}
void integer__float(struct object_st *res, struct integer_st *obj){
    object_set_type(res, REAL_TYPE);
    ((struct real_st *)res->data)->data = obj->data;
}
void integer__str(struct object_st *res, struct integer_st *obj){
    // TODO
}

// Cmp methods
int integer_is_null(const struct integer_st *res) {
    if (res == NULL || res->data == 0) return 1;
    return 0;
}
