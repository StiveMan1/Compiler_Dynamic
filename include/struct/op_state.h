#ifndef OP_STATE_H
#define OP_STATE_H

#include "struct.h"

#define FLAG_NONE       0x00
#define FLAG_BREAK      0x01
#define FLAG_CONTINUE   0x02
#define FLAG_RETURN     0x03

struct op_state{
    struct error_st *error_obj;
    struct object_st *return_obj;

    short flag;

    struct object_st *frame_memory; // Not used. For recursion
    struct array_st *stack_memory;  // Stack of variables
    struct array_st *temp_memory;   // Temporary objects

    struct array_st *code_operations;
};

struct op_state *op_state_new();
void op_state_free(struct op_state *);

void op_state_append(struct op_state *);
void op_state_remove(struct op_state *);

#endif //OP_STATE_H
