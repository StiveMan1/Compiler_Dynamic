#ifndef BOOL_H
#define BOOL_H

#include "gmp.h"
#include "struct.h"

struct bool_st{
    char data;
};
// Standard operations
struct bool_st *bool_new();
void bool_set(struct bool_st *, const struct bool_st *);
void bool_clear(struct bool_st *);
void bool_free(struct bool_st *);
int bool_cmp(const struct bool_st *obj1, const struct bool_st *obj2);

// Class methods
int bool_is_null(const struct bool_st *);

// Print
void print_int(const struct bool_st *);

#endif //BOOL_H
