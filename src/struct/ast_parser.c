#include "struct.h"

struct ast_parser *ast_parser_new(){
    struct ast_parser *res = malloc(sizeof(struct ast_parser));
    res->position = 0;
    res->list = array_new();
    return res;
}
void ast_parser_set(struct ast_parser *res, const struct ast_parser *a){
    res->position = a->position;
    array_set(res->list, a->list);
}
void ast_parser_clear(struct ast_parser *res){
    res->position = 0;
    array_clear(res->list);
}
void ast_parser_free(struct ast_parser *res){
    array_free(res->list);
    free(res);
}

void ast_parser_set_list(struct ast_parser *res, struct la_parser *parser){
    res->position = 0;
    array_set(res->list, parser->list);
}