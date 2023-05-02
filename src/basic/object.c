#include "basic.h"


struct object_math_op object_math = {METHOD_MATH &object__mod, METHOD_MATH &object__and, METHOD_MATH &object__mul,
                                     METHOD_MATH &object__add, METHOD_MATH &object__sub, METHOD_MATH &object__div,
                                     METHOD_MATH &object__xor, METHOD_MATH &object__or, METHOD_MATH &object__ls,
                                     METHOD_MATH &object__rs};
struct object_convert object_convert = {METHOD_CONVERT &object__bool, METHOD_CONVERT &object__int, METHOD_CONVERT &object__float, METHOD_CONVERT &object__str};
struct object_type object_type = {OBJECT_OP, &object_math, &object_convert};
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

// Math Methods
void object__mod(struct object_st *res, struct object_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_mod != NULL) {
        return obj1->type->math->_mod(res, err, obj1->data, obj2);
    }
    object_set_type(err, STRING_TYPE);
    string_set_str(err->data, "Object does not have __mod__ operation", 38);
}
void object__and(struct object_st *res, struct object_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_and != NULL) {
        return obj1->type->math->_and(res, err, obj1->data, obj2);
    }
    object_set_type(err, STRING_TYPE);
    string_set_str(err->data, "Object does not have __and__ operation", 38);
}
void object__mul(struct object_st *res, struct object_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_mul != NULL) {
        return obj1->type->math->_mul(res, err, obj1->data, obj2);
    }
    object_set_type(err, STRING_TYPE);
    string_set_str(err->data, "Object does not have __mul__ operation", 38);
}
void object__add(struct object_st *res, struct object_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_add != NULL) {
        return obj1->type->math->_add(res, err, obj1->data, obj2);
    }
    object_set_type(err, STRING_TYPE);
    string_set_str(err->data, "Object does not have __add__ operation", 38);
}
void object__sub(struct object_st *res, struct object_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_sub != NULL) {
        return obj1->type->math->_sub(res, err, obj1->data, obj2);
    }
    object_set_type(err, STRING_TYPE);
    string_set_str(err->data, "Object does not have __sub__ operation", 38);
}
void object__div(struct object_st *res, struct object_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_div != NULL) {
        return obj1->type->math->_div(res, err, obj1->data, obj2);
    }
    object_set_type(err, STRING_TYPE);
    string_set_str(err->data, "Object does not have __div__ operation", 38);
}
void object__xor(struct object_st *res, struct object_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_xor != NULL) {
        return obj1->type->math->_xor(res, err, obj1->data, obj2);
    }
    object_set_type(err, STRING_TYPE);
    string_set_str(err->data, "Object does not have __xor__ operation", 38);
}
void object__or(struct object_st *res, struct object_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_or != NULL) {
        return obj1->type->math->_or(res, err, obj1->data, obj2);
    }
    object_set_type(err, STRING_TYPE);
    string_set_str(err->data, "Object does not have __or__ operation", 37);
}
void object__ls(struct object_st *res, struct object_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_ls != NULL) {
        return obj1->type->math->_ls(res, err, obj1->data, obj2);
    }
    object_set_type(err, STRING_TYPE);
    string_set_str(err->data, "Object does not have __ls__ operation", 37);
}
void object__rs(struct object_st *res, struct object_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_rs != NULL) {
        return obj1->type->math->_rs(res, err, obj1->data, obj2);
    }
    object_set_type(err, STRING_TYPE);
    string_set_str(err->data, "Object does not have __rs__ operation", 37);
}
void object__neg(struct object_st *res, struct object_st *err, const struct object_st *obj1) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_ne != NULL) {
        return obj1->type->math->_ne(res, err, obj1->data);
    }
    object_set_type(err, STRING_TYPE);
    string_set_str(err->data, "Object does not have __neg__ operation", 38);
}

// Convert Methods
void object__bool(struct object_st *res, struct object_st *err, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj == NULL || obj->type == NULL) {
        object_set_type(res, INTEGER_TYPE);
        ((struct bool_st *)res->data)->data = 0;
        return;
    }
    if (obj->type->convert != NULL && obj->type->convert->_bool != NULL) {
        obj->type->convert->_bool(res, err, obj->data);
    }else{
        object_set_type(res, INTEGER_TYPE);
        ((struct bool_st *)res->data)->data = 1;
        return;
    }
}
void object__int(struct object_st *res, struct object_st *err, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj != NULL && obj->type != NULL && obj->type->convert != NULL && obj->type->convert->_int != NULL) {
        return obj->type->convert->_int(res, err, obj->data);
    }
    object_set_type(err, STRING_TYPE);
    string_set_str(err->data, "Object does not have __int__ operation", 38);
}
void object__float(struct object_st *res, struct object_st *err, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj != NULL && obj->type != NULL && obj->type->convert != NULL && obj->type->convert->_float != NULL) {
        return obj->type->convert->_float(res, err, obj->data);
    }
    object_set_type(err, STRING_TYPE);
    string_set_str(err->data, "Object does not have __float__ operation", 40);
}
void object__str(struct object_st *res, struct object_st *err, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj != NULL && obj->type != NULL && obj->type->convert != NULL && obj->type->convert->_str != NULL) {
        return obj->type->convert->_str(res, err, obj->data);
    }
    object_set_type(err, STRING_TYPE);
    string_set_str(err->data, "Object does not have __str__ operation", 38);
}