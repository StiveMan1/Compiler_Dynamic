#ifndef STRUCT_H
#define STRUCT_H

#include <malloc.h>
#include "struct/object.h"


#define POINTER_SIZE        sizeof(struct object_st *)
#define OBJECT_SIZE         sizeof(struct object_st)


#define OBJECT_OP       (struct object_op) {METHOD_NEW &object_new, METHOD_SET &object_set, METHOD_CLEAR &object_clear, METHOD_FREE &object_free, METHOD_CMP &object_cmp}


extern struct object_type object_type;


#define NONE_TYPE           NULL
#define OBJECT_TYPE         &object_type


#endif //STRUCT_H
