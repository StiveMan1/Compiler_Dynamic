#ifndef STRING_H
#define STRING_H

#include "struct.h"
#include <string.h>

// String Class
struct string_st {
    char *data;
    size_t mx_size, size;
};
// Standard operations
struct string_st *string_new();
void string_set(struct string_st *, const struct string_st *);
void string_clear(struct string_st *);
void string_free(struct string_st *);
int string_cmp(const struct string_st *, const struct string_st *);
int string_is_null(const struct string_st *);

// Class methods
void string_resize(struct string_st *, size_t);
void string_set_str(struct string_st *, const char *, size_t);
void string_concat(struct string_st *, const struct string_st *);

// Print
void print_str(const struct string_st *);

#endif //STRING_H
