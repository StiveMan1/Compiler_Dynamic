#ifndef OP_ATTRIB_H
#define OP_ATTRIB_H

#include "struct.h"

struct op_attrib {
    struct string_st *name;
    struct object_st *data;
};

struct op_attrib *op_attrib_new();
void op_attrib_set(struct op_attrib *, const struct op_attrib *);
void op_attrib_clear(struct op_attrib *);
void op_attrib_free(struct op_attrib *);

void op_attrib_set_name(struct op_attrib *, struct string_st *);
void op_attrib_new_data(struct op_attrib *);
int op_attrib_cmp_name(struct op_attrib *, struct string_st *);
struct object_st *op_attrib_get_data(struct op_attrib *);

#endif //OP_ATTRIB_H
