#ifndef STRUCT_H
#define STRUCT_H

#include <malloc.h>

#include "struct/object.h"
#include "struct/string.h"
#include "struct/integer.h"
#include "struct/bool.h"


#define POINTER_SIZE        sizeof(struct object_st *)
#define OBJECT_SIZE         sizeof(struct object_st)
#define STRING_SIZE         sizeof(struct string_st)
#define INTEGER_SIZE        sizeof(struct integer_st)
#define BOOL_SIZE           sizeof(struct bool_st)


#define OBJECT_OP       (struct object_op) {METHOD_NEW &object_new, METHOD_SET &object_set, METHOD_CLEAR &object_clear, METHOD_FREE &object_free, METHOD_CMP &object_cmp}
#define STRING_OP       (struct object_op) {METHOD_NEW &string_new, METHOD_SET &string_set, METHOD_CLEAR &string_clear, METHOD_FREE &string_free, METHOD_CMP &string_cmp}
#define INTEGER_OP      (struct object_op) {METHOD_NEW &integer_new, METHOD_SET &integer_set, METHOD_CLEAR &integer_clear, METHOD_FREE &integer_free, METHOD_CMP &integer_cmp}
#define BOOL_OP         (struct object_op) {METHOD_NEW &bool_new, METHOD_SET &bool_set, METHOD_CLEAR &bool_clear, METHOD_FREE &bool_free, METHOD_CMP &bool_cmp}


extern struct object_type object_type;
extern struct object_type string_type;
extern struct object_type integer_type;
extern struct object_type bool_type;


#define NONE_TYPE           NULL
#define OBJECT_TYPE         &object_type
#define STRING_TYPE         &string_type
#define INTEGER_TYPE        &integer_type
#define BOOL_TYPE           &bool_type


#endif //STRUCT_H
