#include "struct.h"

struct object_type op_block_type = {OP_BLOCK_OP};
// Create
struct op_block *op_block_new() {
    struct op_block *res = Malloc(OP_BLOCK_SIZE);
    res->type = BlockType_None;
    res->subtype = 0;
    res->data1 = NULL;
    res->data2 = NULL;
    res->count = 0;

    res->pos = 0;
    res->line_num = 0;
    res->line_pos = 0;
    return res;
}
// Set value
void op_block_set(struct op_block *res, const struct op_block *a) {
    if (res->data1 != NULL) object_free(res->data1);
    if (res->data2 != NULL) object_free(res->data2);
    res->data1 = object_copy(a->data1);
    res->data2 = object_copy(a->data2);
    res->type = a->type;
    res->subtype = a->subtype;
    res->count = a->count;

    res->pos = a->pos;
    res->line_num = a->line_num;
    res->line_pos = a->line_pos;
}
// Clear
void op_block_clear(struct op_block *res) {
    if (res->data1 != NULL) object_free(res->data1);
    if (res->data2 != NULL) object_free(res->data2);
    res->data1 = NULL;
    res->data2 = NULL;
    res->type = BlockType_None;
    res->subtype = 0;
    res->count = 0;

    res->pos = 0;
    res->line_num = 0;
    res->line_pos = 0;
}
// Free
void op_block_free(struct op_block *res) {
    if (res->data1 != NULL) object_free(res->data1);
    if (res->data2 != NULL) object_free(res->data2);
    Free(res);
}
// Set position due to node
void op_block_set_position_node(struct op_block *res, struct node_st *node) {
    res->pos = node->pos;
    res->line_num = node->line_num;
    res->line_pos = node->line_pos;
}
// Set position due to token
void op_block_set_position_token(struct op_block *res, struct token_st *token) {
    res->pos = token->pos;
    res->line_num = token->line_num;
    res->line_pos = token->line_pos;
}
// Set position due to block
void op_block_set_position_block(struct op_block *res, struct op_block *block) {
    res->pos = block->pos;
    res->line_num = block->line_num;
    res->line_pos = block->line_pos;
}