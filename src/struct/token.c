#include "struct.h"

struct object_type token_type = {TOKEN_OP};

struct token_st *token_new(){
    struct token_st *res = malloc(TOKEN_SIZE);
    res->type = TokenType_None;
    res->subtype = TokenType_None;

    res->data = NULL;
    res->max_size = res->size = 0;
    return res;
}
void token_set(struct token_st *res, const struct token_st *a){
    res->type = a->type;
    res->subtype = a->subtype;

    token_resize(res, a->size);
    memcpy(res->data, a->data, a->size);
}
void token_clear(struct token_st *res){
    res->type = TokenType_None;
    res->subtype = TokenType_None;

    token_resize(res, 0);
}
void token_free(struct token_st *res){
    if (res->data != NULL) free(res->data);
    free(res);
}

void token_resize(struct token_st *res, size_t size) {
    if (res->data == NULL && size != 0) {
        res->max_size = size;
        res->data = malloc(size + 1);
        if(res->data != NULL) for (size_t i = 0; i < size + 1; i++) res->data[i] = 0;
    } else if (res->max_size < size) {
        res->data = realloc(res->data, size * 2 + 1);
        if(res->data != NULL) for (size_t i = res->max_size; i < size * 2 + 1; i++) res->data[i] = 0;
        res->max_size = size * 2;
    }
    if (res->size > size) {
        for (size_t i = size; i < res->size; i++) {
            res->data[i] = 0;
        }
    }
    res->size = size;
}