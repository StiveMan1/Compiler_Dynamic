# Structures

### object_type
This object is created for save standard(addition, multiplications) function of simple types (int, bool, real, list, ...)
  - `struct object_op self` - struct of initializing and deallocation of structures with can be stored inside `object_st`
  - `struct object_sub *sub` - struct to store function for attribute and subscript 
  - `struct object_convert *convert` - struct to store functions for conversions
  - `struct object_math_op *math` - struct to store functions for math operations

### array_st
This struct is implementation of simple class array
- `struct object_st **data` - array of pointers to mapping the objects of different types
- `size_t size, max_size` - sizes to manage when we need reallocate array of data
  
### bool_st
This struct is implementation of simple class boolean
  - `int data` - the data where store boolean value
  
### darray_st
This struct used for closures in function declaration while program is running
- `struct object_st **data[2]` - the data where store integer value
- `size_t max_size, size` - sizes to manage when we need reallocate array of data
  
### error_st
This struct used for get errors in different stages in the program
  - `int precent` - indicator that error is specified
  - `struct string_st* type` - string to store type of error
  - `struct string_st* message` - string to store message of error
  - `size_t pos, line_num, line_pos` - positioning in source code
  
### func_st
This struct is implementation of simple class boolean
  - `struct object_st *params` - array of parameters
  - `struct object_st *body` - element to saved function body
  - `struct object_st *closure` - map of elements saved for closure
    
### integer_st
This struct is implementation of simple class boolean
  - `long int data` - the data where store integer value

### map_st
This struct is implementation of simple class tuple
  - `struct string_st **names` - array where we store names of the elements
  - `struct object_st **datas` - array where we store data of the elements
  - `size_t size, mx_size` - sizes to manage when we need reallocate array of data
  
### object_st
This struct is implementation of dynamic object witch can be converted and be replaced into different types
  - `struct object_type *type`
  - `short counter` - object counter to manage when we must delete the object
  - `void *data` - pointer to any other structures including `object_st`

### real_st
This struct is implementation of simple class real number (float)
  - `double data` - the data where store real value

### string_st
This struct is implementation of simple class string
  - `char *data` - char array to store string vector
  - `size_t mx_size, size` - sizes to manage when we need reallocate array of data