# Structures

### token_st
- `short type` - save type of token
- `short subtype` - save subtype of token using only for integers and real numbers
- `char *data` - store vector of chars from source code for current token
- `short size, max_size` - sizes to manage when we need reallocate array of data
- `size_t pos, line_num, line_pos` - positioning in source code

### la_parser
- `char *data` - source code
- `size_t position` - positioning in the source code for lexical analyze
- `size_t str_size` - size of source code to not exit from vector
- `size_t current_line` - position of current line for printing errors
- `size_t line_pos` - position where current line start from
- `char scope_buf[MaxBracketNesting]` - scope buffer for check scope problem
- `size_t scope_pos` - position in `scope_buf`
- `struct array_st *list` - resulting list of tokens
- `struct error_st *error_obj` - error of current state
    
### node_st 
This struct maden for ast tree and represent each of the node in that tree.
- `short main_type` - save main type of the node (Expression, Typing, Statement, Operation)
- `short type` - save type of current node 
- `struct object_st *data` - save data from token like integer, real, string 
- `struct object_st *closure` - save closures of node for creating function object
- `struct array_st *next` - list of next nodes in ast tree
- `struct array_st *tokens` - list of tokens using inside this node for operations
- `size_t pos, line_num, line_pos` - positioning in source code 
  
### ast_parser
- `size_t position` - position in list of tokens
- `struct array_st *list` - array of tokens
- `struct error_st *error_obj` - error of current state
- `struct array_st *memory_frame` - list of memory frames to save declared variables in current scopes
- `struct array_st *memory_closure` - list of closures of variables witch not declared inside function scopes
- `struct array_st *expr_stack` - for stack implementation of the semantics
- `int scope_type` - type of current  scope

### op_attrib 
This structure for saving in closures identifier and data with it will store
- `struct string_st *name` - name of the variable
- `struct object_st *data` - pointer to the data of this variable
  
### ob_block 
- `short type` - type of the object 
- `short subtype` - subtype for identify operation correctly
- `size_t count` - counter of objects from temporary memory to use in this block operation
- `struct object_st *data1` - first data object to use wile code interpretation
- `struct object_st *data2` - second data object to use wile code interpretation
- `size_t pos, line_num, line_pos` - positioning in source code

  