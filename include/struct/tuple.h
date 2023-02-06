#ifndef TUPLE_H
#define TUPLE_H

#include "struct.h"

struct tuple_st{
    struct object_st **data;
    size_t size, max_size;
};
// Standard operations
struct tuple_st *tuple_new();
void tuple_set(struct tuple_st *, const struct tuple_st *);
void tuple_clear(struct tuple_st *);
void tuple_free(struct tuple_st *);
int tuple_cmp(const struct tuple_st *, const struct tuple_st *);
int tuple_is_null(const struct tuple_st *);

// Class methods
void tuple_resize(struct tuple_st *, size_t);

// Print
void print_tuple(const struct tuple_st *, int);


#endif //TUPLE_H
