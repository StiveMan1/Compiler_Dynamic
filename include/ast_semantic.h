#ifndef AST_SEMANTIC_H
#define AST_SEMANTIC_H

#include "ast_analyze.h"

struct semantic_state {
    struct array_st *memory_stack;
    struct array_st *memory_size_stack;
};

struct semantic_state *semantic_state_new();

void semantic_state_free(struct semantic_state *res);

void semantic_state_add(struct semantic_state *res, struct object_st *obj);

int semantic_state_find(struct semantic_state *res, struct object_st *obj);

void semantic_state_append_scope(struct semantic_state *res);

void semantic_state_remove_scope(struct semantic_state *res);

void semantic_check(struct semantic_state *state, struct object_st *obj);

#endif //AST_SEMANTIC_H
