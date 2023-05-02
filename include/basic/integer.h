#ifndef INTEGER_H
#define INTEGER_H

#include "basic.h"

struct integer_st{
    long int data;
};
// Standard operations
struct integer_st *integer_new();
void integer_set(struct integer_st *, const struct integer_st *);
void integer_clear(struct integer_st *);
void integer_free(struct integer_st *);
int integer_cmp(const struct integer_st *obj1, const struct integer_st *obj2);

// Class methods
int integer_is_null(const struct integer_st *);

// Math Methods
void integer__mod(struct object_st *, struct object_st *, const struct integer_st *, const struct object_st *);
void integer__and(struct object_st *, struct object_st *, const struct integer_st *, const struct object_st *);
void integer__mul(struct object_st *, struct object_st *, const struct integer_st *, const struct object_st *);
void integer__add(struct object_st *, struct object_st *, const struct integer_st *, const struct object_st *);
void integer__sub(struct object_st *, struct object_st *, const struct integer_st *, const struct object_st *);
void integer__div(struct object_st *, struct object_st *, const struct integer_st *, const struct object_st *);
void integer__xor(struct object_st *, struct object_st *, const struct integer_st *, const struct object_st *);
void integer__or(struct object_st *, struct object_st *, const struct integer_st *, const struct object_st *);
void integer__ls(struct object_st *, struct object_st *, const struct integer_st *, const struct object_st *);
void integer__rs(struct object_st *, struct object_st *, const struct integer_st *, const struct object_st *);
void integer__neg(struct object_st *, struct object_st *, const struct integer_st *);

// Convert Methods
void integer__bool(struct object_st *res, struct object_st *, struct integer_st *obj);
void integer__int(struct object_st *res, struct object_st *, struct integer_st *obj);
void integer__float(struct object_st *res, struct object_st *, struct integer_st *obj);
void integer__str(struct object_st *res, struct object_st *, struct integer_st *obj);

// Print
void print_int(const struct integer_st *);

#endif //INTEGER_H
