#include "basic.h"

struct object_sub string_sub = {METHOD_SUBSCRIPT &string_subscript};
struct object_math_op string_math = {NULL, NULL, METHOD_MATH & string__mul, METHOD_MATH & string__add};
struct object_convert string_convert = {METHOD_CONVERT & string__bool, METHOD_CONVERT & string__int,
                                        METHOD_CONVERT & string__float, METHOD_CONVERT & string__str};
struct object_type string_type = {STRING_OP, &string_sub, &string_convert, &string_math};
// Standard operations
// Create
struct string_st *string_new() {
    struct string_st *res = Malloc(STRING_SIZE);
    res->data = NULL;
    res->mx_size = res->size = 0;
    return res;
}
// Set value
void string_set(struct string_st *res, const struct string_st *a) {
    if (string_is_null(a)) return string_clear(res);
    string_resize(res, a->size);
    memcpy(res->data, a->data, a->size);
}
// Clear
void string_clear(struct string_st *res) {
    string_resize(res, 0);
}
// Free
void string_free(struct string_st *res) {
    if (res->data != NULL) Free(res->data);
    Free(res);
}
// Cmp
int string_cmp(const struct string_st *obj1, const struct string_st *obj2) {
    if (obj1->size > obj2->size) return 1;
    if (obj1->size < obj2->size) return -1;
    return memcmp(obj1->data, obj2->data, obj1->size);
}
int string__cmp(struct error_st *err, struct string_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    struct object_st *temp = object_new();
    object__str(temp, err, obj2);
    if(err->present) {
        object_free(temp);
        return 2;
    }
    int result;
    struct string_st *str = temp->data;
    if (obj1->size > str->size) result = 1;
    else if (obj1->size < str->size) result = -1;
    else result = memcmp(obj1->data, str->data, obj1->size);
    object_free(temp);
    return result;
}
// Check for null
int string_is_null(const struct string_st *res){
    return (res == NULL || res->size == 0);
}

// Class methods
// Change size of a string
void string_resize(struct string_st *res, size_t size) {
    if (res->data == NULL && size != 0) {
        res->mx_size = size;
        res->data = Malloc(size + 1);
        if (res->data != NULL) for (size_t i = 0; i < size + 1; i++) res->data[i] = 0;
    } else if (res->mx_size < size) {
        res->data = Realloc(res->data, size * 2 + 1);
        if (res->data != NULL) for (size_t i = res->mx_size; i < size * 2 + 1; i++) res->data[i] = 0;
        res->mx_size = size * 2;
    }
    if (res->size > size)
        for (size_t i = size; i < res->size; i++) {
            res->data[i] = 0;
        }
    res->size = size;
}
// Set string
void string_set_str(struct string_st *res, const char *str, size_t size) {
    if (res == NULL) return;
    if (str == NULL) return string_clear(res);

    string_resize(res, size);
    memcpy(res->data, str, size);
}
// Concatenate strings
void string_concat(struct string_st *res, const struct string_st *a) {
    if (res == NULL || string_is_null(a)) return;

    size_t _size = res->size;
    string_resize(res, res->size + a->size);
    memcpy(res->data + _size, a->data, a->size);
}

// Math Methods
// Multiplication
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
// Addition
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
// To bool
void string__bool(struct object_st *res, struct error_st *err, const struct string_st *obj) {
    object_set_type(res, BOOL_TYPE);
    ((struct bool_st *)res->data)->data = (obj->size != 0);
}
// To integer
void string__int(struct object_st *res, struct error_st *err, const struct string_st *obj) {
    object_set_type(res, INTEGER_TYPE);
    ((struct integer_st *)res->data)->data = get_integer(obj, err);
}
// To float
void string__float(struct object_st *res, struct error_st *err, const struct string_st *obj) {
    object_set_type(res, REAL_TYPE);
    ((struct real_st *)res->data)->data = get_float(obj, err);
}
// To string
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
