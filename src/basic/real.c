#include "basic.h"

struct object_math_op real_math = {NULL, NULL, METHOD_MATH &real__mul, METHOD_MATH &real__add, METHOD_MATH &real__sub, METHOD_MATH &real__div, NULL, NULL, NULL, NULL, METHOD_CONVERT &real__neg};
struct object_convert real_convert = {METHOD_CONVERT &real__bool, METHOD_CONVERT &real__int, METHOD_CONVERT &real__float, METHOD_CONVERT &real__str};
struct object_type real_type = {REAL_OP, NULL, &real_convert, &real_math};
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

// Math Methods
void real__mul(struct object_st *res, struct object_st *err, const struct real_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    struct object_st *temp = object_new();
    object__float(temp, err, obj2);
    if(err->type != NONE_TYPE) {
        object_free(temp);
        return;
    }
    object_set_type(res, REAL_TYPE);
    ((struct real_st *)res->data)->data = (obj1->data * ((struct real_st *)temp->data)->data);
    object_free(temp);
}
void real__add(struct object_st *res, struct object_st *err, const struct real_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    struct object_st *temp = object_new();
    object__float(temp, err, obj2);
    if(err->type != NONE_TYPE) {
        object_free(temp);
        return;
    }
    object_set_type(res, REAL_TYPE);
    ((struct real_st *)res->data)->data = (obj1->data + ((struct real_st *)temp->data)->data);
    object_free(temp);
}
void real__sub(struct object_st *res, struct object_st *err, const struct real_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    struct object_st *temp = object_new();
    object__float(temp, err, obj2);
    if(err->type != NONE_TYPE) {
        object_free(temp);
        return;
    }
    object_set_type(res, REAL_TYPE);
    ((struct real_st *)res->data)->data = (obj1->data - ((struct real_st *)temp->data)->data);
    object_free(temp);
}
void real__div(struct object_st *res, struct object_st *err, const struct real_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    struct object_st *temp = object_new();
    object__float(temp, err, obj2);
    if(err->type != NONE_TYPE) {
        object_free(temp);
        return;
    }
    if(integer_is_null(temp->data)) {
        object_set_type(err, STRING_TYPE);
        string_set_str(err->data, "Division by zero", 16);
        object_free(temp);
        return;
    }
    object_set_type(res, REAL_TYPE);
    ((struct real_st *)res->data)->data = (obj1->data / ((struct real_st *)temp->data)->data);
    object_free(temp);
}
void real__neg(struct object_st *res, struct object_st *err, const struct real_st *obj1) {
    object_set_type(res, REAL_TYPE);
    ((struct real_st *)res->data)->data = -obj1->data;
}

// Convert Methods
void real__bool(struct object_st *res, struct object_st *err, struct real_st *obj){
    object_set_type(res, BOOL_TYPE);
    ((struct bool_st *)res->data)->data = !real_is_null(obj);
}
void real__int(struct object_st *res, struct object_st *err, struct real_st *obj){
    object_set_type(res, REAL_TYPE);
    ((struct integer_st *)res->data)->data = (int)obj->data;
}
void real__float(struct object_st *res, struct object_st *err, struct real_st *obj){
    object_set_type(res, REAL_TYPE);
    real_set(res->data, obj);
}
void real__str(struct object_st *res, struct object_st *err, struct real_st *obj){
    // TODO
    object_set_type(err, STRING_TYPE);
    string_set_str(err->data, "Not implemented", 15);
}

