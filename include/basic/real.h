#ifndef REAL_H
#define REAL_H

#include "basic.h"

struct real_st{
    double data;
};
// Standard operations
struct real_st *real_new();
void real_set(struct real_st *, const struct real_st *);
void real_clear(struct real_st *);
void real_free(struct real_st *);
int real_cmp(const struct real_st *obj1, const struct real_st *obj2);

// Class methods
int real_is_null(const struct real_st *);

// Math Methods
void real__mul(struct object_st *, struct object_st *, const struct real_st *, const struct object_st *);
void real__add(struct object_st *, struct object_st *, const struct real_st *, const struct object_st *);
void real__sub(struct object_st *, struct object_st *, const struct real_st *, const struct object_st *);
void real__div(struct object_st *, struct object_st *, const struct real_st *, const struct object_st *);
void real__neg(struct object_st *, struct object_st *, const struct real_st *);

// Convert Methods
void real__bool(struct object_st *res, struct object_st *, struct real_st *obj);
void real__int(struct object_st *res, struct object_st *, struct real_st *obj);
void real__float(struct object_st *res, struct object_st *, struct real_st *obj);
void real__str(struct object_st *res, struct object_st *, struct real_st *obj);

// Print
void print_real(const struct real_st *);

#endif //REAL_H
