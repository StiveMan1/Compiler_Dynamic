#include <stdio.h>
#include "struct.h"

// Create new parser for lexical analysis
// It stores information about the input text, 
// the current position of the parser, 
// the current line, the token list, and the error object
struct la_parser *la_parser_new() {
    struct la_parser *res = Malloc(sizeof(struct la_parser));
    res->data = NULL;
    res->position = res->str_size = 0;

    res->current_line = 0;
    res->scope_pos = 0;
    res->line_pos = 0;
    res->list = array_new();
    res->error_obj = error_new();
    return res;
}
// Set value
void la_parser_set(struct la_parser *res, const struct la_parser *a) {
    la_parser_clear(res);
    res->str_size = a->str_size;
    res->data = Malloc(res->str_size);
    memcpy(res->data, a->data, a->str_size);
    error_set(res->error_obj, a->error_obj);
}
// Clear
void la_parser_clear(struct la_parser *res) {
    if (res->data != NULL) Free(res->data);
    res->data = NULL;
    res->position = res->str_size = res->line_pos = 0;

    res->current_line = 0;
    res->scope_pos = 0;
    array_clear(res->list);
    error_clear(res->error_obj);
}
// Free
void la_parser_free(struct la_parser *res) {
    if (res->data != NULL) Free(res->data);
    array_free(res->list);
    error_free(res->error_obj);
    Free(res);
}
// Set string
void la_parser_set_str(struct la_parser *res, char *data, size_t size) {
    la_parser_clear(res);
    res->str_size = size;
    res->data = Malloc(res->str_size + 1);
    memcpy(res->data, data, size);
    res->data[res->str_size] = 0;
}
// Set from file
void la_parser_set_file(struct la_parser *res, char *file_path) {
    la_parser_clear(res);

    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) return;
    fseek(fp, 0, SEEK_END);
    res->str_size = ftell(fp);

    fseek(fp, 0, SEEK_SET);
    res->data = Malloc(res->str_size);
    memset(res->data, 0, res->str_size);
    char c;
    size_t i;
    for (i = 0; i < res->str_size && (c = (char)getc(fp)) != EOF; i++) {
        res->data[i] = c;
    }
    res->str_size = i;
    fclose(fp);
}
// Get error
void la_parser_get_error(struct la_parser *parser, struct error_st *error) {
    error_set(error, parser->error_obj);
}