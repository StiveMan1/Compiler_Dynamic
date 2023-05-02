#include "struct.h"

struct op_state *op_state_new() {
    struct op_state *res = malloc(sizeof(struct op_state));
    res->stack_memory = array_new();
    res->frame_memory = NULL;
    res->temp_memory = array_new();
    res->code_operations = array_new();

    res->error_obj = error_new();
    res->return_obj = NULL;
    return res;
}
void op_state_free(struct op_state *res) {
    array_free(res->stack_memory);

    if(res->frame_memory != NULL) object_free(res->frame_memory);
    array_free(res->temp_memory);
    array_free(res->code_operations);
    error_free(res->error_obj);
    if(res->return_obj != NULL) object_free(res->return_obj);
    free(res);
}