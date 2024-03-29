#include "basic.h"


struct object_math_op object_math = {METHOD_MATH &object__mod, METHOD_MATH &object__and, METHOD_MATH &object__mul,
                                     METHOD_MATH &object__add, METHOD_MATH &object__sub, METHOD_MATH &object__div,
                                     METHOD_MATH &object__xor, METHOD_MATH &object__or, METHOD_MATH &object__ls,
                                     METHOD_MATH &object__rs, METHOD_CONVERT &object__neg};
struct object_convert object_convert = {METHOD_CONVERT &object__bool, METHOD_CONVERT &object__int, METHOD_CONVERT &object__float, METHOD_CONVERT &object__str};
struct object_sub object_sub = {METHOD_SUBSCRIPT &object_subscript, METHOD_ATTRIB &object_attrib};
struct object_type object_type = {OBJECT_OP, &object_sub, &object_convert, &object_math};
// Standard operations
// Create
struct object_st *object_new() {
    struct object_st *res = Malloc(OBJECT_SIZE);
    res->type = NULL;
    res->data = NULL;
    res->counter = 1;
    return res;
}
// Set value 
void object_set(struct object_st *res, const struct object_st *a) {
    if (res == NULL || a == NULL) return;
    object_set_type(res, a->type);
    if (res->type != NULL && res->type->self._set != NULL) res->type->self._set(res->data, (a)->data);
}
// Clear
void object_clear(struct object_st *res) {
    if (res == NULL) return;
    object_set_type(res, NONE_TYPE);
}
// Free
void object_free(struct object_st *res) {
    if (res == NULL || --res->counter > 0) return;
    if (res->data != NULL) {
        if (res->type != NULL && res->type->self._free != NULL)
            res->type->self._free(res->data);
        res->data = NULL;
    }
    Free(res);
}
// Cmp
int object_cmp(struct error_st *err, struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    if (obj2 != NULL && obj2->type == REAL_TYPE && obj1->type != REAL_TYPE) {
        struct object_st *temp = object_new();
        object__float(temp, err, obj1);
        if (err->present) {
            object_free(temp);
            return 2;
        }
        int res = object_cmp(err, temp, obj2);
        object_free(temp);
        return res;
    }
    if (obj1 == obj2) return 0;
    if (obj1->type != NULL && obj1->type->self._cmp != NULL) return obj1->type->self._cmp(err, obj1->data, obj2);
    err->present = 1;
    string_set_str(err->type, INTERPRETER_ERROR, 17);
    string_set_str(err->message, "Object does not have __mul__ operation", 38);
    return 2;
}

// Class methods
struct object_st *object_copy(struct object_st *res) {
    if (res == NULL) return NULL;
    res->counter++;
    return res;
}
// Set type 
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
// Mod
void object__mod(struct object_st *res, struct error_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_mod != NULL) {
        return obj1->type->math->_mod(res, err, obj1->data, obj2);
    }
    
    err->present = 1;
    string_set_str(err->type, INTERPRETER_ERROR, 17);
    string_set_str(err->message, "Object does not have __mod__ operation", 38);
}
// And
void object__and(struct object_st *res, struct error_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_and != NULL) {
        return obj1->type->math->_and(res, err, obj1->data, obj2);
    }
    
    err->present = 1;
    string_set_str(err->type, INTERPRETER_ERROR, 17);
    string_set_str(err->message, "Object does not have __and__ operation", 38);
}
// Multiplication
void object__mul(struct object_st *res, struct error_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    if (obj2 != NULL && obj2->type == REAL_TYPE && obj1->type != REAL_TYPE) {
        struct object_st *temp = object_new();
        object__float(temp, err, obj1);
        if (err->present) {
            object_free(temp);
            return;
        }
        object__mul(res, err, temp, obj2);
        object_free(temp);
        return;
    }
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_mul != NULL) {
        return obj1->type->math->_mul(res, err, obj1->data, obj2);
    }
    
    err->present = 1;
    string_set_str(err->type, INTERPRETER_ERROR, 17);
    string_set_str(err->message, "Object does not have __mul__ operation", 38);
}
// Addition
void object__add(struct object_st *res, struct error_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    if (obj2 != NULL && obj2->type == REAL_TYPE && obj1->type != REAL_TYPE) {
        struct object_st *temp = object_new();
        object__float(temp, err, obj1);
        if (err->present) {
            object_free(temp);
            return;
        }
        object__add(res, err, temp, obj2);
        object_free(temp);
        return;
    }
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_add != NULL) {
        return obj1->type->math->_add(res, err, obj1->data, obj2);
    }
    
    err->present = 1;
    string_set_str(err->type, INTERPRETER_ERROR, 17);
    string_set_str(err->message, "Object does not have __add__ operation", 38);
}
// Substraction
void object__sub(struct object_st *res, struct error_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    if (obj2 != NULL && obj2->type == REAL_TYPE && obj1->type != REAL_TYPE) {
        struct object_st *temp = object_new();
        object__float(temp, err, obj1);
        if (err->present) {
            object_free(temp);
            return;
        }
        object__sub(res, err, temp, obj2);
        object_free(temp);
        return;
    }
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_sub != NULL) {
        return obj1->type->math->_sub(res, err, obj1->data, obj2);
    }
    
    err->present = 1;
    string_set_str(err->type, INTERPRETER_ERROR, 17);
    string_set_str(err->message, "Object does not have __sub__ operation", 38);
}
// Division
void object__div(struct object_st *res, struct error_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    if (obj2 != NULL && obj2->type == REAL_TYPE && obj1->type != REAL_TYPE) {
        struct object_st *temp = object_new();
        object__float(temp, err, obj1);
        if (err->present) {
            object_free(temp);
            return;
        }
        object__div(res, err, temp, obj2);
        object_free(temp);
        return;
    }
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_div != NULL) {
        return obj1->type->math->_div(res, err, obj1->data, obj2);
    }
    
    err->present = 1;
    string_set_str(err->type, INTERPRETER_ERROR, 17);
    string_set_str(err->message, "Object does not have __div__ operation", 38);
}
// XOR
void object__xor(struct object_st *res, struct error_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_xor != NULL) {
        return obj1->type->math->_xor(res, err, obj1->data, obj2);
    }
    
    err->present = 1;
    string_set_str(err->type, INTERPRETER_ERROR, 17);
    string_set_str(err->message, "Object does not have __xor__ operation", 38);
}
// OR
void object__or(struct object_st *res, struct error_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_or != NULL) {
        return obj1->type->math->_or(res, err, obj1->data, obj2);
    }

    err->present = 1;
    string_set_str(err->type, INTERPRETER_ERROR, 17);
    string_set_str(err->message, "Object does not have __or__ operation", 37);
}
// LS
void object__ls(struct object_st *res, struct error_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_ls != NULL) {
        return obj1->type->math->_ls(res, err, obj1->data, obj2);
    }

    err->present = 1;
    string_set_str(err->type, INTERPRETER_ERROR, 17);
    string_set_str(err->message, "Object does not have __ls__ operation", 37);
}
// RS
void object__rs(struct object_st *res, struct error_st *err, const struct object_st *obj1, const struct object_st *obj2) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_rs != NULL) {
        return obj1->type->math->_rs(res, err, obj1->data, obj2);
    }

    err->present = 1;
    string_set_str(err->type, INTERPRETER_ERROR, 17);
    string_set_str(err->message, "Object does not have __rs__ operation", 37);
}
// Negative
void object__neg(struct object_st *res, struct error_st *err, const struct object_st *obj1) {
    while (obj1 != NULL && obj1->type == OBJECT_TYPE) obj1 = obj1->data;
    if (obj1 != NULL && obj1->type != NULL && obj1->type->math != NULL && obj1->type->math->_ne != NULL) {
        return obj1->type->math->_ne(res, err, obj1->data);
    }

    err->present = 1;
    string_set_str(err->type, INTERPRETER_ERROR, 17);
    string_set_str(err->message, "Object does not have __neg__ operation", 38);
}

// Convert Methods
// To bool
void object__bool(struct object_st *res, struct error_st *err, const struct object_st *obj) {
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
// To integer
void object__int(struct object_st *res, struct error_st *err, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj != NULL && obj->type != NULL && obj->type->convert != NULL && obj->type->convert->_int != NULL) {
        return obj->type->convert->_int(res, err, obj->data);
    }

    err->present = 1;
    string_set_str(err->type, INTERPRETER_ERROR, 17);
    string_set_str(err->message, "Object does not have __int__ operation", 38);
}
// To float
void object__float(struct object_st *res, struct error_st *err, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj != NULL && obj->type != NULL && obj->type->convert != NULL && obj->type->convert->_float != NULL) {
        return obj->type->convert->_float(res, err, obj->data);
    }

    err->present = 1;
    string_set_str(err->type, INTERPRETER_ERROR, 17);
    string_set_str(err->message, "Object does not have __float__ operation", 40);
}
// To string
void object__str(struct object_st *res, struct error_st *err, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj != NULL && obj->type != NULL && obj->type->convert != NULL && obj->type->convert->_str != NULL) {
        return obj->type->convert->_str(res, err, obj->data);
    }
    object_set_type(res, STRING_TYPE);
    string_set_str(res->data, "empty", 5);
}

// Sub method
struct object_st *object_subscript(struct error_st *err, struct object_st *obj, const struct object_st *obj2) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj != NULL && obj->type != NULL && obj->type->sub != NULL && obj->type->sub->_subscript != NULL) {
        return obj->type->sub->_subscript(err, obj->data, obj2);
    }
    
    err->present = 1;
    string_set_str(err->type, INTERPRETER_ERROR, 17);
    string_set_str(err->message, "Object does not have __subscript__ operation", 48);
    return NULL;
}
struct object_st *object_attrib(struct error_st *err, const struct object_st *obj, const struct object_st *str) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj != NULL && obj->type != NULL && obj->type->sub != NULL && obj->type->sub->_attrib != NULL) {
        return obj->type->sub->_attrib(err, obj->data, str);
    }

    err->present = 1;
    string_set_str(err->type, INTERPRETER_ERROR, 17);
    string_set_str(err->message, "Object does not have __attrib__ operation", 45);
    return NULL;
}