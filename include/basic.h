#ifndef BASIC_H
#define BASIC_H

#include "memory/memory.h"

#include "basic/object.h"
#include "basic/string.h"
#include "basic/integer.h"
#include "basic/bool.h"
#include "basic/real.h"
#include "basic/array.h"
#include "basic/darray.h"
#include "basic/map.h"
#include "basic/error.h"
#include "basic/func.h"


#define POINTER_SIZE        sizeof(struct object_st *)
#define OBJECT_SIZE         sizeof(struct object_st)
#define STRING_SIZE         sizeof(struct string_st)
#define INTEGER_SIZE        sizeof(struct integer_st)
#define BOOL_SIZE           sizeof(struct bool_st)
#define REAL_SIZE           sizeof(struct real_st)
#define ARRAY_SIZE          sizeof(struct array_st)
#define DARRAY_SIZE         sizeof(struct darray_st)
#define MAP_SIZE            sizeof(struct map_st)
#define FUNC_SIZE           sizeof(struct func_st)



#define OBJECT_OP       (struct object_op) {METHOD_NEW &object_new, METHOD_SET &object_set, METHOD_CLEAR &object_clear, METHOD_FREE &object_free, METHOD_CMP &object_cmp}
#define STRING_OP       (struct object_op) {METHOD_NEW &string_new, METHOD_SET &string_set, METHOD_CLEAR &string_clear, METHOD_FREE &string_free, METHOD_CMP &string__cmp}
#define INTEGER_OP      (struct object_op) {METHOD_NEW &integer_new, METHOD_SET &integer_set, METHOD_CLEAR &integer_clear, METHOD_FREE &integer_free, METHOD_CMP &integer__cmp}
#define BOOL_OP         (struct object_op) {METHOD_NEW &bool_new, METHOD_SET &bool_set, METHOD_CLEAR &bool_clear, METHOD_FREE &bool_free, METHOD_CMP &bool__cmp}
#define REAL_OP         (struct object_op) {METHOD_NEW &real_new, METHOD_SET &real_set, METHOD_CLEAR &real_clear, METHOD_FREE &real_free, METHOD_CMP &real__cmp}
#define ARRAY_OP        (struct object_op) {METHOD_NEW &array_new, METHOD_SET &array_set, METHOD_CLEAR &array_clear, METHOD_FREE &array_free, METHOD_CMP &array__cmp}
#define DARRAY_OP       (struct object_op) {METHOD_NEW &darray_new, METHOD_SET &darray_set, METHOD_CLEAR &darray_clear, METHOD_FREE &darray_free}
#define MAP_OP          (struct object_op) {METHOD_NEW &map_new, METHOD_SET &map_set, METHOD_CLEAR &map_clear, METHOD_FREE &map_free, METHOD_CMP &map__cmp}
#define FUNC_OP         (struct object_op) {METHOD_NEW &func_new, METHOD_SET &func_set, METHOD_CLEAR &func_clear, METHOD_FREE &func_free}

extern struct object_type object_type;
extern struct object_type string_type;
extern struct object_type integer_type;
extern struct object_type bool_type;
extern struct object_type real_type;
extern struct object_type array_type;
extern struct object_type darray_type;
extern struct object_type map_type;
extern struct object_type func_type;


#define NONE_TYPE           NULL
#define OBJECT_TYPE         &object_type
#define STRING_TYPE         &string_type
#define INTEGER_TYPE        &integer_type
#define BOOL_TYPE           &bool_type
#define REAL_TYPE           &real_type
#define ARRAY_TYPE          &array_type
#define DARRAY_TYPE         &darray_type
#define MAP_TYPE            &map_type
#define FUNC_TYPE           &func_type


#endif //BASIC_H
