#ifndef COMPILER_DYNAMIC_AST_SCOPE_IDENT_H
#define COMPILER_DYNAMIC_AST_SCOPE_IDENT_H

#include "struct.h"


#define ScopeType_None  0x00
#define ScopeType_Func  0x01
#define ScopeType_Loop  0x02

#define SemanticError_Ident     0x01
#define SemanticError_Return    0x02

struct sm_state {
    struct array_st *memory_frame;
    struct array_st *memory_closure;
    struct array_st *expr_stack;

    int type;
    int error;
};
struct sm_state *sm_state_new();
void sm_state_free(struct sm_state *);
void sm_state_save_type(struct sm_state *, struct node_st *);
void sm_state_remove_frame(struct sm_state *);

struct object_st *sm_state_set_ident(struct sm_state *, struct object_st *);
struct object_st *sm_state_get_ident(struct sm_state *, struct object_st *);

int semantic_scan(struct object_st *);

#endif //COMPILER_DYNAMIC_AST_SCOPE_IDENT_H
