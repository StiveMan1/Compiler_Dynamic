#ifndef ERROR_H
#define ERROR_H

#include "basic.h"

struct error_st {
    int present;
    char *type;
    char *message;
    int line;
    int line_pos;
};
// Standard operations
struct error_st *error_new();
void error_set(struct error_st *, const struct error_st *);
void error_clear(struct error_st *);
void error_free(struct error_st *);
int error_cmp(const struct error_st *, const struct error_st *);
int error_is_null(const struct error_st *);

// Print
void print_error(const struct error_st *, int);


#endif //ERROR_H
