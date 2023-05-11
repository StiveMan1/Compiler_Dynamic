#ifndef OP_OBJECT_H
#define OP_OBJECT_H

#include "struct.h"

struct node_st;
struct func_st{
    struct object_st *data;

    // Class Object
    struct object_st *class;
    struct map_st *attr;

    // Closure
    struct darray_st *closure;
};

struct func_st *func_new();
void func_set(struct func_st *res, const struct func_st *a);
void func_clear(struct func_st *res);
void func_free(struct func_st *res);

void func_set_data(struct func_st *, struct object_st *);
void func_set_function(struct func_st *, struct node_st *);
struct object_st *func_get_attrib(const struct func_st *, struct string_st *);
struct object_st *func_set_attrib(const struct func_st *, const struct string_st *);


#endif //OP_OBJECT_H
