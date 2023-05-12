#include "struct.h"

// Create
struct imp_parser *imp_parser_new() {
    struct imp_parser *res = Malloc(sizeof(struct imp_parser));
    res->stack_memory = array_new();
    res->temp_memory = array_new();
    res->code_operations = array_new();

    res->error_obj = error_new();
    res->return_obj = NULL;
    res->tree = object_new();
    object_set_type(res->tree, NODE_TYPE);
    return res;
}
// Free
void imp_parser_free(struct imp_parser *res) {
    array_free(res->stack_memory);

    array_free(res->temp_memory);
    array_free(res->code_operations);
    error_free(res->error_obj);
    if(res->return_obj != NULL) object_free(res->return_obj);
    object_free(res->tree);
    Free(res);
}
// Set tree
void imp_parser_set_tree(struct imp_parser *res, struct ast_parser *parser) {
    node_set(res->tree->data, parser->tree);
}
// Get error
void imp_parser_get_error(struct imp_parser *res, struct error_st *error) {
    error_set(error, res->error_obj);
}