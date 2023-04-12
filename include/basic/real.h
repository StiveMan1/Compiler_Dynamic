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

// Print
void print_real(const struct real_st *);

#endif //REAL_H
