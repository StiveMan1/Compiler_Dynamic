#ifndef OP_OBJECT_H
#define OP_OBJECT_H

#include "struct.h"

struct op_object{
    struct object_st *data;

    // Class Object
    struct object_st *class;
    struct map_st *attr;

    // Closure
    struct darray_st *closure;
};

struct op_object *op_object_new();
void op_object_set(struct op_object *res, const struct op_object *a);
void op_object_clear(struct op_object *res);
void op_object_free(struct op_object *res);

void op_object_set_data(struct op_object *, struct object_st *);
void op_object_set_function(struct op_object *, struct node_st *);
struct object_st *op_object_get_attrib(const struct op_object *, struct string_st *);
struct object_st *op_object_set_attrib(const struct op_object *, const struct string_st *);

// Math Methods
void op_object__mod(struct object_st *, struct object_st *, const struct op_object *, struct object_st *);
void op_object__and(struct object_st *, struct object_st *, const struct op_object *, struct object_st *);
void op_object__mul(struct object_st *, struct object_st *, const struct op_object *, struct object_st *);
void op_object__add(struct object_st *, struct object_st *, const struct op_object *, struct object_st *);
void op_object__sub(struct object_st *, struct object_st *, const struct op_object *, struct object_st *);
void op_object__div(struct object_st *, struct object_st *, const struct op_object *, struct object_st *);
void op_object__xor(struct object_st *, struct object_st *, const struct op_object *, struct object_st *);
void op_object__or(struct object_st *, struct object_st *, const struct op_object *, struct object_st *);
void op_object__ls(struct object_st *, struct object_st *, const struct op_object *, struct object_st *);
void op_object__rs(struct object_st *, struct object_st *, const struct op_object *, struct object_st *);
void op_object__neg(struct object_st *, struct object_st *, const struct op_object *);

// Convert Methods
void op_object__bool(struct object_st *, struct object_st *, const struct op_object *);
void op_object__int(struct object_st *, struct object_st *, const struct op_object *);
void op_object__float(struct object_st *, struct object_st *, const struct op_object *);
void op_object__str(struct object_st *, struct object_st *, const struct op_object *);

// Sub method
struct object_st *op_object_subscript(struct object_st *, struct op_object *, const struct object_st *);
struct object_st *op_object_attrib(struct object_st *, const struct op_object *, const struct string_st *);


#endif //OP_OBJECT_H
