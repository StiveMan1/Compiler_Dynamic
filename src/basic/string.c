#include "basic.h"

struct object_sub string_sub = {METHOD_SUBSCRIPT &string_subscript};
struct object_math_op string_math = {NULL, NULL, METHOD_MATH & string__mul, METHOD_MATH & string__add};
struct object_convert string_convert = {METHOD_CONVERT & string__bool, METHOD_CONVERT & string__int,
                                        METHOD_CONVERT & string__float, METHOD_CONVERT & string__str};
struct object_type string_type = {STRING_OP, &string_sub, &string_convert, &string_math};
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

// Math Methods
void string__mul(struct object_st *res, struct error_st *err, const struct string_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    struct object_st *temp = object_new();
    object__int(temp, err, obj2);
    if(err->present) {
        object_free(temp);
        return;
    }
    object_set_type(res, STRING_TYPE);
    unsigned int count = ((struct integer_st *)temp->data)->data;
    for (unsigned int i = 0; i < count; i++)
        string_concat(res->data, obj1);
    object_free(temp);
}
void string__add(struct object_st *res, struct error_st *err, const struct string_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    struct object_st *temp = object_new();
    object__str(temp, err, obj2);
    if(err->present) {
        object_free(temp);
        return;
    }
    object_set_type(res, STRING_TYPE);
    string_set(res->data, obj1);
    string_concat(res->data, temp->data);
    object_free(temp);
}

// Convert Methods
void string__bool(struct object_st *res, struct error_st *err, const struct string_st *obj) {
    object_set_type(res, BOOL_TYPE);
    ((struct bool_st *)res->data)->data = (obj->size != 0);
}
void string__int(struct object_st *res, struct error_st *err, const struct string_st *obj) {
    // TODO
    object_set_type(res->data, INTEGER_TYPE);
    struct  integer_st *result = NULL;
    result->data = 0;
    for(size_t i = 0; i < obj->size; i++){
        if('0' <= obj->data[i] && obj->data[i] <= '9'){
            result->data = result->data * 10 + (obj->data[i] - '0');
        } else {
            err->present = 1;
            string_set_str(err->type, INTERPRETER_ERROR, 15);
            string_set_str(err->message, "Not implemented", 15);
            return ;
        }
    }
    integer_set(res->data, result);
    integer_free(result);
//    err->present = 1;
//    string_set_str(err->type, INTERPRETER_ERROR, 15);
//    string_set_str(err->message, "Not implemented", 15);

}
void string__float(struct object_st *res, struct error_st *err, const struct string_st *obj) {
    // TODO

    object_set_type(res->data, REAL_TYPE);
    struct  real_st *result = NULL;
    short dot = 0;
    for(size_t i = 0; i < obj->size; i++){
        if('0' <= obj->data[i] && obj->data[i] <= '9') {
            result->data = result->data * 10 + (obj->data[i] - '0');

        } else if(obj->data[i] == '.') {
            dot = 1;
        } else {
            err->present = 1;
            string_set_str(err->type, INTERPRETER_ERROR, 15);
            string_set_str(err->message, "Not implemented", 15);
            return ;
        }
    }
    if(dot == 0 || obj->data[obj->size - 1] == '.'){
        err->present = 1;
        string_set_str(err->type, INTERPRETER_ERROR, 15);
        string_set_str(err->message, "Not implemented", 15);
        return ;
    }
    for(size_t i = obj->size; i > 0; i--){
        if(obj->data[i-1] == '.'){
            break;
        }
        result->data /= 10;
    }
    real_set(res->data, result);
    real_free(result);
//    err->present = 1;
//    string_set_str(err->type, INTERPRETER_ERROR, 15);
//    string_set_str(err->message, "Not implemented", 15);
}
void string__str(struct object_st *res, struct error_st *err, const struct string_st *obj) {
    object_set_type(res, STRING_TYPE);
    string_set(res->data, obj);
}


// Convert Methods
struct object_st *string_subscript(struct error_st *err, struct string_st *str, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    struct object_st *temp = object_new();
    object__int(temp, err, obj);
    if(err->present) {
        object_free(temp);
        return NULL;
    }
    struct object_st *res = object_new();
    size_t position = ((struct integer_st *)temp->data)->data % str->size;
    object_set_type(res, STRING_TYPE);
    string_set_str(res->data, str->data + position, 1);
    object_free(temp);
    return res;
}
