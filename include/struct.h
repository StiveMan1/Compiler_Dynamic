#ifndef STRUCT_H
#define STRUCT_H

#include "basic.h"

#include "struct/la_parser.h"
#include "struct/ast_parser.h"
#include "struct/op_state.h"

#include "struct/token.h"
#include "struct/ast_node.h"
#include "struct/op_block.h"
#include "struct/op_attrib.h"


#define TOKEN_SIZE          sizeof(struct token_st)
#define NODE_SIZE           sizeof(struct node_st)
#define OP_BLOCK_SIZE       sizeof(struct op_block)
#define OP_ATTRIB_SIZE      sizeof(struct op_attrib)



#define TOKEN_OP        (struct object_op) {METHOD_NEW &token_new, METHOD_SET &token_set, METHOD_CLEAR &token_clear, METHOD_FREE &token_free, NULL}
#define NODE_OP         (struct object_op) {METHOD_NEW &node_new, METHOD_SET &node_set, METHOD_CLEAR &node_clear, METHOD_FREE &node_free, NULL}
#define OP_BLOCK_OP     (struct object_op) {METHOD_NEW &op_block_new, METHOD_SET &op_block_set, METHOD_CLEAR &op_block_clear, METHOD_FREE &op_block_free, NULL}
#define OP_ATTRIB_OP    (struct object_op) {METHOD_NEW &op_attrib_new, METHOD_SET &op_attrib_set, METHOD_CLEAR &op_attrib_clear, METHOD_FREE &op_attrib_free, NULL}

extern struct object_type token_type;
extern struct object_type node_type;
extern struct object_type op_block_type;
extern struct object_type op_attrib_type;


#define TOKEN_TYPE          &token_type
#define NODE_TYPE           &node_type
#define OP_BLOCK_TYPE       &op_block_type
#define OP_ATTRIB_TYPE      &op_attrib_type


#endif //STRUCT_H
