#ifndef ARRAY_H
#define ARRAY_H

#include "struct.h"

struct array_st{
    struct object_st **data;
    size_t size, max_size;
};
// Standard operations
struct array_st *array_new();
void array_set(struct array_st *, const struct array_st *);
void array_clear(struct array_st *);
void array_free(struct array_st *);
int array_cmp(const struct array_st *, const struct array_st *);
int array_is_null(const struct array_st *);

// Class methods
void array_resize(struct array_st *, size_t);
void array_append(struct array_st *, struct object_st* );
void array_concat(struct array_st *, const struct array_st *);
void array_add_new(struct array_st *, struct object_type *);

void array_sort(struct array_st *);

// Print
void print_array(const struct array_st *, int);


#endif //ARRAY_H
