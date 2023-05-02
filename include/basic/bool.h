#ifndef BOOL_H
#define BOOL_H

#include "basic.h"

struct bool_st{
    int data;
};
// Standard operations
struct bool_st *bool_new();
void bool_set(struct bool_st *, const struct bool_st *);
void bool_clear(struct bool_st *);
void bool_free(struct bool_st *);
int bool_cmp(const struct bool_st *obj1, const struct bool_st *obj2);

// Class methods
int bool_is_null(const struct bool_st *);

// Math Methods
void bool__mod(struct object_st *, struct object_st *, const struct bool_st *, const struct object_st *);
void bool__and(struct object_st *, struct object_st *, const struct bool_st *, const struct object_st *);
void bool__mul(struct object_st *, struct object_st *, const struct bool_st *, const struct object_st *);
void bool__add(struct object_st *, struct object_st *, const struct bool_st *, const struct object_st *);
void bool__sub(struct object_st *, struct object_st *, const struct bool_st *, const struct object_st *);
void bool__div(struct object_st *, struct object_st *, const struct bool_st *, const struct object_st *);
void bool__xor(struct object_st *, struct object_st *, const struct bool_st *, const struct object_st *);
void bool__or(struct object_st *, struct object_st *, const struct bool_st *, const struct object_st *);
void bool__ls(struct object_st *, struct object_st *, const struct bool_st *, const struct object_st *);
void bool__rs(struct object_st *, struct object_st *, const struct bool_st *, const struct object_st *);
void bool__neg(struct object_st *, struct object_st *, const struct bool_st *);

// Convert Methods
void bool__bool(struct object_st *res, struct object_st *, struct bool_st *obj);
void bool__int(struct object_st *res, struct object_st *, struct bool_st *obj);
void bool__float(struct object_st *res, struct object_st *, struct bool_st *obj);
void bool__str(struct object_st *res, struct object_st *, struct bool_st *obj);

// Print
void print_bool(const struct bool_st *);

#endif //BOOL_H
