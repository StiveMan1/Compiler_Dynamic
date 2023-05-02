#include "struct.h"
struct object_math_op op_object_math = {METHOD_MATH &op_object__mod, METHOD_MATH &op_object__and, METHOD_MATH &op_object__mul,
                                     METHOD_MATH &op_object__add, METHOD_MATH &op_object__sub, METHOD_MATH &op_object__div,
                                     METHOD_MATH &op_object__xor, METHOD_MATH &op_object__or, METHOD_MATH &op_object__ls,
                                     METHOD_MATH &op_object__rs, METHOD_CONVERT &op_object__neg};
struct object_convert op_object_convert = {METHOD_CONVERT &op_object__bool, METHOD_CONVERT &op_object__int, METHOD_CONVERT &op_object__float, METHOD_CONVERT &op_object__str};
struct object_sub op_object_sub = {METHOD_SUBSCRIPT &op_object_subscript, METHOD_ATTRIB &op_object_attrib};
struct object_type op_object_type = {OP_OBJECT_OP, &op_object_sub, &op_object_convert, &op_object_math};

struct op_object *op_object_new() {
    struct op_object *res = malloc(OP_OBJECT_SIZE);
    res->data = NULL;

    res->class = NULL;
    res->attr = map_new();

    res->closure = darray_new();

    return res;
}
void op_object_set(struct op_object *res, const struct op_object *a) {
    op_object_clear(res);

    res->data = object_copy(a->data);

    res->class = object_copy(a->class);
    map_set(res->attr, a->attr);

    darray_set(res->closure, a->closure);
}
void op_object_clear(struct op_object *res) {
    if(res->data != NULL) object_free(res->data);
    res->data = NULL;

    if(res->class != NULL) object_free(res->class);
    res->class = NULL;
    map_clear(res->attr);

    darray_clear(res->closure);
}
void op_object_free(struct op_object *res) {
    if(res->data != NULL) object_free(res->data);

    if(res->class != NULL) object_free(res->class);
    map_free(res->attr);

    darray_free(res->closure);
    free(res);
}

void op_object_set_function(struct op_object *res, struct node_st *node) {
    op_object_clear(res);

    struct object_st *obj = NULL;
    struct string_st *attr_name = string_new();

    string_set_str(attr_name, "__params__", 10);
    obj = map_set_elm(res->attr, attr_name->data, attr_name->size);
    object_set(obj, node->next->data[0]);
    object_free(obj);


    string_set_str(attr_name, "__call__", 7);
    obj = map_set_elm(res->attr, attr_name->data, attr_name->size);
    object_set(obj, node->next->data[1]);
    object_free(obj);

    string_set_str(attr_name, "__closure__", 11);
    obj = map_set_elm(res->attr, attr_name->data, attr_name->size);
    object_set(obj, node->closure);
    if (obj->data != NULL) {
        struct darray_st *closure = obj->data;
        struct object_st *temp = NULL;
        for (size_t i = 0 ; i < closure->size; i++) {
            temp = closure->data[1][i];
            closure->data[1][i] = object_copy(((struct op_attrib *)temp->data)->data);
            object_free(temp);
        }
    }
    object_free(obj);

    string_free(attr_name);
}
struct object_st *op_object_get_attrib(const struct op_object *res, struct string_st *name) {
    return map_get_elm(res->attr, name->data, name->size);
}
struct object_st *op_object_set_attrib(const struct op_object *res, const struct string_st *name) {
    return map_set_elm(res->attr, name->data, name->size);
}

void function_call(struct object_st *res, struct error_st *err, struct object_st *func, struct array_st *args);
// Math Methods
void op_object__mod(struct object_st *res, struct error_st *err, const struct op_object *obj1, struct object_st *obj2) {
    struct object_st *func = NULL;
    {
        struct string_st *function_name = string_new();
        string_set_str(function_name, "__mod__", 7);
        func = op_object_get_attrib(obj1, function_name);
        string_free(function_name);
    }
    if (func == NULL) {
        err->present = 1;
        string_set_str(err->type, INTERPRETER_ERROR, 15);
        string_set_str(err->message, "Object does not have __mod__ operation", 38);
        return;
    }
    struct array_st *args = array_new();
    {
        struct object_st *obj_temp = object_new();
        object_set_type(obj_temp, OP_OBJECT_TYPE);
        op_object_set(obj_temp->data, obj1);
        array_append(args, obj_temp);
        object_free(obj_temp);
    }
    array_append(args, obj2);
    function_call(res, err, func, args);
    array_free(args);
}
void op_object__and(struct object_st *res, struct error_st *err, const struct op_object *obj1, struct object_st *obj2) {
    struct object_st *func = NULL;
    {
        struct string_st *function_name = string_new();
        string_set_str(function_name, "__add__", 7);
        func = op_object_get_attrib(obj1, function_name);
        string_free(function_name);
    }
    if (func == NULL) {
        err->present = 1;
        string_set_str(err->type, INTERPRETER_ERROR, 15);
        string_set_str(err->message, "Object does not have __and__ operation", 38);
        return;
    }
    struct array_st *args = array_new();
    {
        struct object_st *obj_temp = object_new();
        object_set_type(obj_temp, OP_OBJECT_TYPE);
        op_object_set(obj_temp->data, obj1);
        array_append(args, obj_temp);
        object_free(obj_temp);
    }
    array_append(args, obj2);
    function_call(res, err, func, args);
    array_free(args);
}
void op_object__mul(struct object_st *res, struct error_st *err, const struct op_object *obj1, struct object_st *obj2) {
    struct object_st *func = NULL;
    {
        struct string_st *function_name = string_new();
        string_set_str(function_name, "__mul__", 7);
        func = op_object_get_attrib(obj1, function_name);
        string_free(function_name);
    }
    if (func == NULL) {
        err->present = 1;
        string_set_str(err->type, INTERPRETER_ERROR, 15);
        string_set_str(err->message, "Object does not have __mul__ operation", 38);
        return;
    }
    struct array_st *args = array_new();
    {
        struct object_st *obj_temp = object_new();
        object_set_type(obj_temp, OP_OBJECT_TYPE);
        op_object_set(obj_temp->data, obj1);
        array_append(args, obj_temp);
        object_free(obj_temp);
    }
    array_append(args, obj2);
    function_call(res, err, func, args);
    array_free(args);
}
void op_object__add(struct object_st *res, struct error_st *err, const struct op_object *obj1, struct object_st *obj2) {
    struct object_st *func = NULL;
    {
        struct string_st *function_name = string_new();
        string_set_str(function_name, "__add__", 7);
        func = op_object_get_attrib(obj1, function_name);
        string_free(function_name);
    }
    if (func == NULL) {
        err->present = 1;
        string_set_str(err->type, INTERPRETER_ERROR, 15);
        string_set_str(err->message, "Object does not have __add__ operation", 38);
        return;
    }
    struct array_st *args = array_new();
    {
        struct object_st *obj_temp = object_new();
        object_set_type(obj_temp, OP_OBJECT_TYPE);
        op_object_set(obj_temp->data, obj1);
        array_append(args, obj_temp);
        object_free(obj_temp);
    }
    array_append(args, obj2);
    function_call(res, err, func, args);
    array_free(args);
}
void op_object__sub(struct object_st *res, struct error_st *err, const struct op_object *obj1, struct object_st *obj2) {
    struct object_st *func = NULL;
    {
        struct string_st *function_name = string_new();
        string_set_str(function_name, "__sub__", 7);
        func = op_object_get_attrib(obj1, function_name);
        string_free(function_name);
    }
    if (func == NULL) {
        err->present = 1;
        string_set_str(err->type, INTERPRETER_ERROR, 15);
        string_set_str(err->message, "Object does not have __sub__ operation", 38);
        return;
    }
    struct array_st *args = array_new();
    {
        struct object_st *obj_temp = object_new();
        object_set_type(obj_temp, OP_OBJECT_TYPE);
        op_object_set(obj_temp->data, obj1);
        array_append(args, obj_temp);
        object_free(obj_temp);
    }
    array_append(args, obj2);
    function_call(res, err, func, args);
    array_free(args);
}
void op_object__div(struct object_st *res, struct error_st *err, const struct op_object *obj1, struct object_st *obj2) {
    struct object_st *func = NULL;
    {
        struct string_st *function_name = string_new();
        string_set_str(function_name, "__div__", 7);
        func = op_object_get_attrib(obj1, function_name);
        string_free(function_name);
    }
    if (func == NULL) {
        err->present = 1;
        string_set_str(err->type, INTERPRETER_ERROR, 15);
        string_set_str(err->message, "Object does not have __div__ operation", 38);
        return;
    }
    struct array_st *args = array_new();
    {
        struct object_st *obj_temp = object_new();
        object_set_type(obj_temp, OP_OBJECT_TYPE);
        op_object_set(obj_temp->data, obj1);
        array_append(args, obj_temp);
        object_free(obj_temp);
    }
    array_append(args, obj2);
    function_call(res, err, func, args);
    array_free(args);
}
void op_object__xor(struct object_st *res, struct error_st *err, const struct op_object *obj1, struct object_st *obj2) {
    struct object_st *func = NULL;
    {
        struct string_st *function_name = string_new();
        string_set_str(function_name, "__xor__", 7);
        func = op_object_get_attrib(obj1, function_name);
        string_free(function_name);
    }
    if (func == NULL) {
        err->present = 1;
        string_set_str(err->type, INTERPRETER_ERROR, 15);
        string_set_str(err->message, "Object does not have __xor__ operation", 38);
        return;
    }
    struct array_st *args = array_new();
    {
        struct object_st *obj_temp = object_new();
        object_set_type(obj_temp, OP_OBJECT_TYPE);
        op_object_set(obj_temp->data, obj1);
        array_append(args, obj_temp);
        object_free(obj_temp);
    }
    array_append(args, obj2);
    function_call(res, err, func, args);
    array_free(args);
}
void op_object__or(struct object_st *res, struct error_st *err, const struct op_object *obj1, struct object_st *obj2) {
    struct object_st *func = NULL;
    {
        struct string_st *function_name = string_new();
        string_set_str(function_name, "__or__", 6);
        func = op_object_get_attrib(obj1, function_name);
        string_free(function_name);
    }
    if (func == NULL) {
        err->present = 1;
        string_set_str(err->type, INTERPRETER_ERROR, 15);
        string_set_str(err->message, "Object does not have __or__ operation", 37);
        return;
    }
    struct array_st *args = array_new();
    {
        struct object_st *obj_temp = object_new();
        object_set_type(obj_temp, OP_OBJECT_TYPE);
        op_object_set(obj_temp->data, obj1);
        array_append(args, obj_temp);
        object_free(obj_temp);
    }
    array_append(args, obj2);
    function_call(res, err, func, args);
    array_free(args);
}
void op_object__ls(struct object_st *res, struct error_st *err, const struct op_object *obj1, struct object_st *obj2) {
    struct object_st *func = NULL;
    {
        struct string_st *function_name = string_new();
        string_set_str(function_name, "__ls__", 6);
        func = op_object_get_attrib(obj1, function_name);
        string_free(function_name);
    }
    if (func == NULL) {
        err->present = 1;
        string_set_str(err->type, INTERPRETER_ERROR, 15);
        string_set_str(err->message, "Object does not have __ls__ operation", 37);
        return;
    }
    struct array_st *args = array_new();
    {
        struct object_st *obj_temp = object_new();
        object_set_type(obj_temp, OP_OBJECT_TYPE);
        op_object_set(obj_temp->data, obj1);
        array_append(args, obj_temp);
        object_free(obj_temp);
    }
    array_append(args, obj2);
    function_call(res, err, func, args);
    array_free(args);
}
void op_object__rs(struct object_st *res, struct error_st *err, const struct op_object *obj1, struct object_st *obj2) {
    struct object_st *func = NULL;
    {
        struct string_st *function_name = string_new();
        string_set_str(function_name, "__rs__", 6);
        func = op_object_get_attrib(obj1, function_name);
        string_free(function_name);
    }
    if (func == NULL) {
        err->present = 1;
        string_set_str(err->type, INTERPRETER_ERROR, 15);
        string_set_str(err->message, "Object does not have __rs__ operation", 37);
        return;
    }
    struct array_st *args = array_new();
    {
        struct object_st *obj_temp = object_new();
        object_set_type(obj_temp, OP_OBJECT_TYPE);
        op_object_set(obj_temp->data, obj1);
        array_append(args, obj_temp);
        object_free(obj_temp);
    }
    array_append(args, obj2);
    function_call(res, err, func, args);
    array_free(args);
}
void op_object__neg(struct object_st *res, struct error_st *err, const struct op_object *obj1) {
    struct object_st *func = NULL;
    {
        struct string_st *function_name = string_new();
        string_set_str(function_name, "__neg__", 7);
        func = op_object_get_attrib(obj1, function_name);
        string_free(function_name);
    }
    if (func == NULL) {
        err->present = 1;
        string_set_str(err->type, INTERPRETER_ERROR, 15);
        string_set_str(err->message, "Object does not have __neg__ operation", 38);
        return;
    }
    struct array_st *args = array_new();
    {
        struct object_st *obj_temp = object_new();
        object_set_type(obj_temp, OP_OBJECT_TYPE);
        op_object_set(obj_temp->data, obj1);
        array_append(args, obj_temp);
        object_free(obj_temp);
    }
    function_call(res, err, func, args);
    array_free(args);
}

// Convert Methods
void op_object__bool(struct object_st *res, struct error_st *err, const struct op_object *obj) {
    struct object_st *func = NULL;
    {
        struct string_st *function_name = string_new();
        string_set_str(function_name, "__bool__", 8);
        func = op_object_get_attrib(obj, function_name);
        string_free(function_name);
    }
    if (func == NULL) {
        object_set_type(res, INTEGER_TYPE);
        ((struct bool_st *)res->data)->data = 1;
        return;
    }
    struct array_st *args = array_new();
    {
        struct object_st *obj_temp = object_new();
        object_set_type(obj_temp, OP_OBJECT_TYPE);
        op_object_set(obj_temp->data, obj);
        array_append(args, obj_temp);
        object_free(obj_temp);
    }
    function_call(res, err, func, args);
    array_free(args);
}
void op_object__int(struct object_st *res, struct error_st *err, const struct op_object *obj) {
    struct object_st *func = NULL;
    {
        struct string_st *function_name = string_new();
        string_set_str(function_name, "__int__", 7);
        func = op_object_get_attrib(obj, function_name);
        string_free(function_name);
    }
    if (func == NULL) {
        err->present = 1;
        string_set_str(err->type, INTERPRETER_ERROR, 15);
        string_set_str(err->message, "Object does not have __int__ operation", 38);
        return;
    }
    struct array_st *args = array_new();
    {
        struct object_st *obj_temp = object_new();
        object_set_type(obj_temp, OP_OBJECT_TYPE);
        op_object_set(obj_temp->data, obj);
        array_append(args, obj_temp);
        object_free(obj_temp);
    }
    function_call(res, err, func, args);
    array_free(args);
}
void op_object__float(struct object_st *res, struct error_st *err, const struct op_object *obj) {
    struct object_st *func = NULL;
    {
        struct string_st *function_name = string_new();
        string_set_str(function_name, "__real__", 8);
        func = op_object_get_attrib(obj, function_name);
        string_free(function_name);
    }
    if (func == NULL) {
        err->present = 1;
        string_set_str(err->type, INTERPRETER_ERROR, 15);
        string_set_str(err->message, "Object does not have __float__ operation", 40);
        return;
    }
    struct array_st *args = array_new();
    {
        struct object_st *obj_temp = object_new();
        object_set_type(obj_temp, OP_OBJECT_TYPE);
        op_object_set(obj_temp->data, obj);
        array_append(args, obj_temp);
        object_free(obj_temp);
    }
    function_call(res, err, func, args);
    array_free(args);
}
void op_object__str(struct object_st *res, struct error_st *err, const struct op_object *obj) {
    struct object_st *func = NULL;
    {
        struct string_st *function_name = string_new();
        string_set_str(function_name, "__str__", 7);
        func = op_object_get_attrib(obj, function_name);
        string_free(function_name);
    }
    if (func == NULL) {
        err->present = 1;
        string_set_str(err->type, INTERPRETER_ERROR, 15);
        string_set_str(err->message, "Object does not have __str__ operation", 38);
        return;
    }
    struct array_st *args = array_new();
    {
        struct object_st *obj_temp = object_new();
        object_set_type(obj_temp, OP_OBJECT_TYPE);
        op_object_set(obj_temp->data, obj);
        array_append(args, obj_temp);
        object_free(obj_temp);
    }
    function_call(res, err, func, args);
    array_free(args);
}

// Sub method
struct object_st *op_object_subscript(struct error_st *err, struct op_object *obj, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    struct object_st *temp = object_new();
    object__str(temp, err, obj2);
    if(err->present) {
        object_free(temp);
        return object_new();
    }
    struct object_st *res = op_object_set_attrib(obj, temp->data);
    object_free(temp);
    return res;
}
struct object_st *op_object_attrib(struct error_st *err, const struct op_object *obj, const struct string_st *str) {
    return op_object_set_attrib(obj, str);
}