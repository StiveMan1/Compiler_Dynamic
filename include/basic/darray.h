#ifndef DARRAY_H
#define DARRAY_H

#include "basic.h"

struct darray_st {
    struct object_st **data[2];
    size_t max_size, size;
};

struct darray_st *darray_new();
void darray_set(struct darray_st *, const struct darray_st *);
void darray_clear(struct darray_st *);
void darray_free(struct darray_st *);
int darray_cmp(const struct darray_st *, const struct darray_st *);
int darray_is_null(const struct darray_st *);

void darray_resize(struct darray_st *, size_t);
void darray_append(struct darray_st *, struct object_st *, struct object_st *);

// Print
void print_darray(const struct darray_st *, int);

#endif //DARRAY_H
