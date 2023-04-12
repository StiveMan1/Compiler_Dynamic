#include "basic.h"

struct object_math_op bool_math = {METHOD_MATH &bool__mod, METHOD_MATH &bool__and, METHOD_MATH &bool__mul, METHOD_MATH &bool__add, METHOD_MATH &bool__sub, METHOD_MATH &bool__div, METHOD_MATH &bool__xor, METHOD_MATH &bool__or, METHOD_MATH &bool__ls, METHOD_MATH &bool__rs, METHOD_CONVERT &bool__neg};
struct object_convert bool_convert = {METHOD_CONVERT &bool__bool, METHOD_CONVERT &bool__int, METHOD_CONVERT &bool__float, METHOD_CONVERT &bool__str};
struct object_type bool_type = {BOOL_OP, &bool_math, &bool_convert};
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

// Math Methods
void bool__mod(struct object_st *res, const struct bool_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != BOOL_TYPE) return;
    object_set_type(res, BOOL_TYPE);
    ((struct bool_st *)res->data)->data = obj1->data % ((struct bool_st *)obj2->data)->data;
}
void bool__and(struct object_st *res, const struct bool_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != BOOL_TYPE) return;
    object_set_type(res, BOOL_TYPE);
    ((struct bool_st *)res->data)->data = (obj1->data & ((struct bool_st *)obj2->data)->data);
}
void bool__mul(struct object_st *res, const struct bool_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != BOOL_TYPE) return;
    object_set_type(res, BOOL_TYPE);
    ((struct bool_st *)res->data)->data = obj1->data * ((struct bool_st *)obj2->data)->data;
}
void bool__add(struct object_st *res, const struct bool_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != BOOL_TYPE) return;
    object_set_type(res, BOOL_TYPE);
    ((struct bool_st *)res->data)->data = obj1->data + ((struct bool_st *)obj2->data)->data;
}
void bool__sub(struct object_st *res, const struct bool_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != BOOL_TYPE) return;
    object_set_type(res, BOOL_TYPE);
    ((struct bool_st *)res->data)->data = obj1->data - ((struct bool_st *)obj2->data)->data;
}
void bool__div(struct object_st *res, const struct bool_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != BOOL_TYPE) return;
    object_set_type(res, BOOL_TYPE);
    ((struct bool_st *)res->data)->data = obj1->data / ((struct bool_st *)obj2->data)->data;
}
void bool__xor(struct object_st *res, const struct bool_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != BOOL_TYPE) return;
    object_set_type(res, BOOL_TYPE);
    ((struct bool_st *)res->data)->data = (obj1->data ^ ((struct bool_st *)obj2->data)->data);
}
void bool__or(struct object_st *res, const struct bool_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != BOOL_TYPE) return;
    object_set_type(res, BOOL_TYPE);
    ((struct bool_st *)res->data)->data = (obj1->data | ((struct bool_st *)obj2->data)->data);
}
void bool__ls(struct object_st *res, const struct bool_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != BOOL_TYPE) return;
    object_set_type(res, BOOL_TYPE);
    ((struct bool_st *)res->data)->data = (obj1->data << ((struct bool_st *)obj2->data)->data);
}
void bool__rs(struct object_st *res, const struct bool_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != BOOL_TYPE) return;
    object_set_type(res, BOOL_TYPE);
    ((struct bool_st *)res->data)->data = (obj1->data >> ((struct bool_st *)obj2->data)->data);
}
void bool__neg(struct object_st *res, const struct bool_st *obj1) {
    object_set_type(res, BOOL_TYPE);
    ((struct bool_st *)res->data)->data = -obj1->data;
}

// Convert Methods
void bool__bool(struct object_st *res, struct bool_st *obj){
    object_set_type(res, BOOL_TYPE);
    bool_set(res->data, obj);
}
void bool__int(struct object_st *res, struct bool_st *obj){
    object_set_type(res, INTEGER_TYPE);
    ((struct integer_st *)res->data)->data = obj->data;
}
void bool__float(struct object_st *res, struct bool_st *obj){
    object_set_type(res, REAL_TYPE);
    ((struct real_st *)res->data)->data = obj->data;
}
void bool__str(struct object_st *res, struct bool_st *obj){
    // TODO
}

