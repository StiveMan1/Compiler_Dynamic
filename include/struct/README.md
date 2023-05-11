# Structures

- token_st
    - `short type`
    - `short subtype`
    - `short size, max_size`
    - `size_t pos, line_num, line_pos`

- la_parser
    - `char *data`
    - `size_t position`
    -  `size_t str_size`
    - `size_t current_line`
    -  `size_t line_pos`
    - `char scope_buf[MaxBracketNesting]`
    - `size_t scope_pos`
    - `struct array_st *list`
    - `struct error_st *error_obj`
    
- op_state
  - `struct error_st *error_obj`
  - `struct object_st *return_obj`
  - `short flag`
  - `struct object_st *frame_memory`
  - `struct array_st *stack_memory`  
  - `struct array_st *temp_memory`
  - `struct array_st *code_operations`