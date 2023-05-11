#ifndef OP_STATE_H
#define OP_STATE_H

#include "struct.h"

#define FLAG_NONE       0x00
#define FLAG_BREAK      0x01
#define FLAG_CONTINUE   0x02
#define FLAG_RETURN     0x03

struct imp_parser{
    struct error_st *error_obj;
    struct object_st *return_obj;
    struct object_st *tree;

    short flag;

    struct object_st *frame_memory; // Not used. For recursion
    struct array_st *stack_memory;  // Stack of variables
    struct array_st *temp_memory;   // Temporary objects

    struct array_st *code_operations;
};

struct imp_parser *imp_parser_new();
void imp_parser_free(struct imp_parser *);

void imp_parser_set_tree(struct imp_parser *, struct ast_parser *);
void imp_parser_get_error(struct imp_parser *, struct error_st *);

void imp_parser_append(struct imp_parser *);
void imp_parser_remove(struct imp_parser *);

#endif //OP_STATE_H
