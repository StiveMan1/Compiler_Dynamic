# Structures

- object_op
    - `void *(*_new)()`
    - `void (*_set)(void *, const void *)`
    - `void (*_clear)(void *)`
    - `void (*_free)(void *)`
    - `int (*_cmp)(struct error_st *, void *, const void *)`

- object_sub
    - `struct object_st *(*_subscript)(struct error_st *, void *, const struct object_st *)`
    - `struct object_st *(*_attrib)(struct error_st *, void *, const struct object_st *)`

- object_math_op
    - `void (*_mod)(struct object_st *, struct error_st *, const void *, const struct object_st *)` 
    - `void (*_and)(struct object_st *, struct error_st *, const void *, const struct object_st *)`
    - `void (*_mul)(struct object_st *, struct error_st *, const void *, const struct object_st *)`
    - `void (*_add)(struct object_st *, struct error_st *, const void *, const struct object_st *)`
    - `void (*_sub)(struct object_st *, struct error_st *, const void *, const struct object_st *)`
    - `void (*_div)(struct object_st *, struct error_st *, const void *, const struct object_st *)`
    - `void (*_xor)(struct object_st *, struct error_st *, const void *, const struct object_st *)`
    - `void (*_or)(struct object_st *, struct error_st *, const void *, const struct object_st *)`
    - `void (*_ls)(struct object_st *, struct error_st *, const void *, const struct object_st *)`
    - `void (*_rs)(struct object_st *, struct error_st *, const void *, const struct object_st *)`
    - `void (*_ne)(struct object_st *, struct error_st *, const void *)`


- object_type 
    - `struct object_op self`
    - `struct object_sub *sub`
    - `struct object_convert *convert`
    - `struct object_math_op *math`

- array_st
    - `struct object_st **data`
    - `size_t size, max_size`
    
- bool_st
    - `int data`
    
- darray_st 
    - `struct object_st **data[2]`
    - `size_t max_size, size`
    
- error_st 
    - `int precent`
    - `struct string_st* type`
    - `struct string_st* message`
    - `size_t pos, line_num, line_pos`
    
- func_st
    - `struct object_st *data`
    - `struct object_st *class`
    - `struct map_st *attr`
    - `struct darray_st *closure`
    
- integer_st 
    - `long int data`
    
- map_st 
    - `struct string_st **names`
    - `struct object_st **datas`
    - `size_t size, mx_size`
    
- object_st 
    - `struct object_type *type`
    - `short counter`
    - `void *data`
    
- real_st 
    - `double data`
    
- string_st
    - `char *data`
    - `size_t mx_size, size`