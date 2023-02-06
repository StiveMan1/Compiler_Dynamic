#ifndef INTEGER_H
#define INTEGER_H

#include "gmp.h"
#include "struct.h"

struct integer_st{
    long int num;
};
// Standard operations
struct integer_st *integer_new();
void integer_set(struct integer_st *, const struct integer_st *);
void integer_clear(struct integer_st *);
void integer_free(struct integer_st *);
int integer_cmp(const struct integer_st *obj1, const struct integer_st *obj2);

// Class methods
int integer_is_null(const struct integer_st *);

// Print
void print_int(const struct integer_st *);

#endif //INTEGER_H
