#ifndef ERROR_H
#define ERROR_H

#include "basic.h"

#define LEXICAL_ANALYSIS_ERROR      "Lexical error"
#define SYNTAX_ANALYSIS_ERROR       "Syntactical error"
#define SEMANTIC_ANALYSIS_ERROR     "Semantic error"
#define INTERPRETER_ERROR           "Interpreter error"

struct error_st {
    int present;
    struct string_st* type;
    struct string_st* message;

    size_t pos, line_num, line_pos;
};
// Standard operations
struct error_st *error_new();
void error_set(struct error_st *, const struct error_st *);
void error_clear(struct error_st *);
void error_free(struct error_st *);
int error_cmp(const struct error_st *, const struct error_st *);
int error_is_null(const struct error_st *);

// Print
void print_error(const struct error_st *);

// Class functions
void error_fill_in(struct error_st *res, char* type, char* message, size_t line_start, size_t line, size_t line_pos);


#endif //ERROR_H
