#include "struct.h"

struct object_type op_block_type = {OP_BLOCK_OP};

struct op_block *op_block_new() {
    struct op_block *res = malloc(OP_BLOCK_SIZE);
    res->type = BlockType_None;
    res->subtype = 0;
    res->data1 = NULL;
    res->data2 = NULL;
    res->count = 0;
    return res;
}
void op_block_set(struct op_block *res, const struct op_block *a) {
    if (res->data1 != NULL) object_free(res->data1);
    if (res->data2 != NULL) object_free(res->data2);
    res->data1 = object_copy(a->data1);
    res->data2 = object_copy(a->data2);
    res->type = a->type;
    res->subtype = a->subtype;
    res->count = a->count;
}
void op_block_clear(struct op_block *res) {
    if (res->data1 != NULL) object_free(res->data1);
    if (res->data2 != NULL) object_free(res->data2);
    res->data1 = NULL;
    res->data2 = NULL;
    res->type = BlockType_None;
    res->subtype = 0;
    res->count = 0;
}
void op_block_free(struct op_block *res) {
    if (res->data1 != NULL) object_free(res->data1);
    if (res->data2 != NULL) object_free(res->data2);
    free(res);
}