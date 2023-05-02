#ifndef OP_BLOCK_H
#define OP_BLOCK_H

#include "struct.h"

#define BlockType_None 0x00

struct op_block{
    short type;
    short subtype;
    size_t count;
    struct object_st *data1;
    struct object_st *data2;

    size_t pos, line_num, line_pos;
};


struct op_block *op_block_new();
void op_block_set(struct op_block *, const struct op_block *);
void op_block_clear(struct op_block *);
void op_block_free(struct op_block *);

void op_block_set_position_node(struct op_block *, struct node_st *);
void op_block_set_position_token(struct op_block *, struct token_st *);
void op_block_set_position_block(struct op_block *, struct op_block *);

#endif //OP_BLOCK_H
