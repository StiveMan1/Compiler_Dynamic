#include "basic.h"

struct object_sub list_sub = {METHOD_SUBSCRIPT &array_subscript};
struct object_math_op array_math = {NULL, NULL, METHOD_MATH &array__mul, METHOD_MATH &array__add};
struct object_convert array_convert = {NULL, NULL, NULL, METHOD_CONVERT &array__str};
struct object_type array_type = {ARRAY_OP, &list_sub,  &array_convert, &array_math};
// Standard operations
// Create
struct array_st *array_new() {
    struct array_st *res = Malloc(ARRAY_SIZE);
    res->data = NULL;
    res->max_size = res->size = 0;
    return res;
}
// Set value
void array_set(struct array_st *res, const struct array_st *a) {
    if (a == NULL) return;
    array_resize(res, 0);
    array_resize(res, a->size);
    for (size_t i = 0; i < a->size; i++) res->data[i] = object_copy(a->data[i]);
}
// Clear
void array_clear(struct array_st *res) {
    array_resize(res, 0);
}
// Free
void array_free(struct array_st *res) {
    array_resize(res, 0);
    if (res->data != NULL) Free(res->data);
    Free(res);
}
// Cmp
int array__cmp(struct error_st *err, struct array_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    if(obj2->type != ARRAY_TYPE) {
        err->present = 1;
        string_set_str(err->type, INTERPRETER_ERROR, 17);
        string_set_str(err->message, "array can't compare with non array type", 39);
        return 2;
    }
    struct array_st *array = obj2->data;
    if (obj1->size > array->size) return 1;
    if (obj1->size < array->size) return -1;
    int result = 0;
    for (size_t i = 0; i < obj1->size && result == 0; i++) {
        result = object_cmp(err, obj1->data[i], array->data[i]);
        if (err->present) return 2;
    }
    return result;
}
// Check for null
int array_is_null(const struct array_st *res) {
    return (res == NULL || res->size == 0);
}

// Class methods
// Change size of the array
void array_resize(struct array_st *res, size_t size) {
    if (res->data == NULL && size != 0) {
        res->max_size = size;
        res->data = Malloc(POINTER_SIZE * size);
        for (size_t i = 0; i < size; i++) res->data[i] = NULL;
    } else if (res->max_size < size) {
        res->data = Realloc(res->data, POINTER_SIZE * size * 2);
        for (size_t i = res->max_size; i < size * 2; i++) res->data[i] = NULL;
        res->max_size = size * 2;
    }
    if (res->size > size) {
        for (size_t i = size; i < res->size; i++) {
            if (res->data[i] != NULL) object_free(res->data[i]);
            res->data[i] = NULL;
        }
    }
    res->size = size;
}
// Append the array
void array_append(struct array_st *res, struct object_st *obj) {
    if (res == NULL) return;

    array_resize(res, res->size + 1);
    res->data[res->size - 1] = object_copy(obj);
}
// Concatenate arrays
void array_concat(struct array_st *res, const struct array_st *a) {
    if (res == NULL || array_is_null(a)) return;

    size_t _size = res->size;
    array_resize(res, res->size + a->size);
    for (size_t i = 0; i < a->size; i++) {
        res->data[_size + i] = object_copy(a->data[i]);
    }
}
// Add new element to the array
void array_add_new(struct array_st *res, struct object_type *type) {
    if (res == NULL) return;

    array_resize(res, res->size + 1);
    res->data[res->size - 1] = object_new();
    object_set_type(res->data[res->size - 1], type);
}
// Remove last element of the array
void array_remove_last(struct array_st *res) {
    if(res->size <= 0) return;
    array_resize(res, res->size - 1);
}
// Return last element of the array
struct object_st *array_get_last(struct array_st *res) {
    if(res->size <= 0) return NULL;
    return res->data[res->size - 1];
}


// Math Methods
// Multiplication
void array__mul(struct object_st *res, struct error_st *err, const struct array_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    struct object_st *temp = object_new();
    object__int(temp, err, obj2);
    if(err->present) {
        object_free(temp);
        return;
    }
    object_set_type(res, ARRAY_TYPE);
    unsigned int count = ((struct integer_st *)temp->data)->data;
    for (unsigned int i = 0; i < count; i++)
        array_concat(res->data, obj1);
    object_free(temp);
}
// Addition
void array__add(struct object_st *res, struct error_st *err, const struct array_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = res->data;
    if (obj2 == NULL || obj2->type != ARRAY_TYPE) {
        err->present = 1;
        string_set_str(err->type, INTERPRETER_ERROR, 17);
        string_set_str(err->message, "list dont have operation add with non list type", 47);
        return;
    }
    object_set_type(res, ARRAY_TYPE);
    array_set(res->data, obj1);
    array_concat(res->data, obj2->data);
}

// Convert Methods
// To string
void array__str(struct object_st *res, struct error_st *err, const struct array_st *obj){
    object_set_type(res, STRING_TYPE);
    string_set_str(res->data, "[", 1);
    struct object_st *temp = object_new();
    for(size_t i = 0; i < obj->size; i++){
        object__str(temp, err, obj->data[i]);
        if (err->present) {
            object_free(temp);
            return;
        }
        string_concat(res->data, temp->data);
        if (i + 1 < obj->size) {
            string_set_str(temp->data, ", ", 2);
            string_concat(res->data, temp->data);
        }
    }
    string_set_str(temp->data, "]", 1);
    string_concat(res->data, temp->data);
    object_free(temp);
}

// Convert Methods
struct object_st *array_subscript(struct error_st *err, struct array_st *list, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    struct object_st *temp = object_new();
    object__int(temp, err, obj);
    if(err->present) {
        object_free(temp);
        return NULL;
    }
    size_t position = ((struct integer_st *)temp->data)->data;
    object_free(temp);
    if (position > list->size) array_resize(list, position);
    return object_copy(list->data[position % list->size]);
}
