#ifndef OP_CLOSURE_H
#define OP_CLOSURE_H

#include "struct.h"

struct op_closure {
    struct object_st **data[2];
    size_t max_size, size;
};

struct op_closure *op_closure_new();
void op_closure_set(struct op_closure *, const struct op_closure *);
void op_closure_clear(struct op_closure *);
void op_closure_free(struct op_closure *);

void op_closure_closure_resize(struct op_closure *, size_t);
void op_closure_append(struct op_closure *, struct object_st *, struct object_st *);

#endif //OP_CLOSURE_H
