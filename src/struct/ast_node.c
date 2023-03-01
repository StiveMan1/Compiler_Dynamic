#include "struct.h"
#include "struct/ast_node.h"

struct object_type node_type = {NODE_OP};

struct node_st *node_new(){
    struct node_st *res = malloc(NODE_SIZE);
    res->main_type = MainType_None;
    res->type = ExprType_None;

    res->data = NULL;
    res->next = array_new();
    res->tokens = array_new();
    return res;
}
void node_set(struct node_st *res, const struct node_st *a){
    res->main_type = a->main_type;
    res->type = a->type;
    if(res->data != NULL) object_free(res->data);
    res->data = object_copy(a->data);
    array_set(res->next, a->next);
    array_set(res->tokens, a->tokens);
}
void node_clear(struct node_st *res){
    res->main_type = MainType_None;
    res->type = ExprType_None;
    if(res->data != NULL) object_free(res->data);
    res->data = NULL;
    array_clear(res->next);
    array_clear(res->tokens);
}
void node_free(struct node_st *res){
    if(res->data != NULL) object_free(res->data);
    array_free(res->next);
    array_free(res->tokens);
    free(res);
}