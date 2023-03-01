#ifndef STRUCT_H
#define STRUCT_H

#include <malloc.h>

#include "struct/object.h"
#include "struct/string.h"
#include "struct/integer.h"
#include "struct/bool.h"
#include "struct/real.h"
#include "struct/array.h"
#include "struct/tuple.h"
#include "struct/token.h"
#include "struct/la_parser.h"
#include "struct/ast_node.h"
#include "struct/ast_parser.h"


#define POINTER_SIZE        sizeof(struct object_st *)
#define OBJECT_SIZE         sizeof(struct object_st)
#define STRING_SIZE         sizeof(struct string_st)
#define INTEGER_SIZE        sizeof(struct integer_st)
#define BOOL_SIZE           sizeof(struct bool_st)
#define REAL_SIZE           sizeof(struct real_st)
#define ARRAY_SIZE          sizeof(struct array_st)
#define TUPLE_SIZE          sizeof(struct tuple_st)
#define TOKEN_SIZE          sizeof(struct token_st)
#define NODE_SIZE           sizeof(struct node_st)



#define OBJECT_OP       (struct object_op) {METHOD_NEW &object_new, METHOD_SET &object_set, METHOD_CLEAR &object_clear, METHOD_FREE &object_free, METHOD_CMP &object_cmp}
#define STRING_OP       (struct object_op) {METHOD_NEW &string_new, METHOD_SET &string_set, METHOD_CLEAR &string_clear, METHOD_FREE &string_free, METHOD_CMP &string_cmp}
#define INTEGER_OP      (struct object_op) {METHOD_NEW &integer_new, METHOD_SET &integer_set, METHOD_CLEAR &integer_clear, METHOD_FREE &integer_free, METHOD_CMP &integer_cmp}
#define BOOL_OP         (struct object_op) {METHOD_NEW &bool_new, METHOD_SET &bool_set, METHOD_CLEAR &bool_clear, METHOD_FREE &bool_free, METHOD_CMP &bool_cmp}
#define REAL_OP         (struct object_op) {METHOD_NEW &real_new, METHOD_SET &real_set, METHOD_CLEAR &real_clear, METHOD_FREE &real_free, METHOD_CMP &real_cmp}
#define ARRAY_OP        (struct object_op) {METHOD_NEW &array_new, METHOD_SET &array_set, METHOD_CLEAR &array_clear, METHOD_FREE &array_free, METHOD_CMP &array_cmp}
#define TUPLE_OP        (struct object_op) {METHOD_NEW &tuple_new, METHOD_SET &tuple_set, METHOD_CLEAR &tuple_clear, METHOD_FREE &tuple_free, METHOD_CMP &tuple_cmp}
#define TOKEN_OP        (struct object_op) {METHOD_NEW &token_new, METHOD_SET &token_set, METHOD_CLEAR &token_clear, METHOD_FREE &token_free, NULL}
#define NODE_OP         (struct object_op) {METHOD_NEW &node_new, METHOD_SET &node_set, METHOD_CLEAR &node_clear, METHOD_FREE &node_free, NULL}

extern struct object_type object_type;
extern struct object_type string_type;
extern struct object_type integer_type;
extern struct object_type bool_type;
extern struct object_type real_type;
extern struct object_type array_type;
extern struct object_type tuple_type;
extern struct object_type token_type;
extern struct object_type node_type;


#define NONE_TYPE           NULL
#define OBJECT_TYPE         &object_type
#define STRING_TYPE         &string_type
#define INTEGER_TYPE        &integer_type
#define BOOL_TYPE           &bool_type
#define REAL_TYPE           &real_type
#define ARRAY_TYPE          &array_type
#define TUPLE_TYPE          &tuple_type
#define TOKEN_TYPE          &token_type
#define NODE_TYPE           &node_type


#endif //STRUCT_H
