#include "struct.h"

struct object_type token_type = {TOKEN_OP};

struct token_st *token_new(){
    struct token_st *res = Malloc(TOKEN_SIZE);
    res->type = TokenType_None;
    res->subtype = TokenType_None;

    res->data = NULL;
    res->max_size = res->size = 0;
    res->line_num = res->line_pos = res->pos = 0;
    return res;
}
void token_set(struct token_st *res, const struct token_st *a){
    res->type = a->type;
    res->subtype = a->subtype;
    res->line_num = a->line_num;
    res->line_pos = a->line_pos;
    res->pos = a->pos;

    token_resize(res, a->size);
    memcpy(res->data, a->data, a->size);
}
// Clearing data
void token_clear(struct token_st *res){
    res->type = TokenType_None;
    res->subtype = TokenType_None;
    res->line_num = res->line_pos = res->pos = 0;

    token_resize(res, 0);
}
// Completely deleting the object
void token_free(struct token_st *res){
    if (res->data != NULL) Free(res->data);
    Free(res);
}

// Setting the size of array for the token data
void token_resize(struct token_st *res, size_t size) {
    if (res->data == NULL && size != 0) {
        res->max_size = size;
        res->data = Malloc(size + 1);
        if(res->data != NULL) for (size_t i = 0; i < size + 1; i++) res->data[i] = 0;
    } else if (res->max_size < size) {
        res->data = Realloc(res->data, size * 2 + 1);
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
void token_set_pos(struct token_st *res, struct la_parser *parser) {
    res->line_num = parser->current_line;
    res->line_pos = parser->line_pos;
    res->pos = parser->position;
}