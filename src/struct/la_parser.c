#include "struct.h"

struct la_parser *la_parser_new() {
    struct la_parser *res = malloc(sizeof(struct la_parser));
    res->data = NULL;
    res->position = res->str_size = 0;

    res->current_line = 0;
    res->scope_pos = 0;
    res->list = array_new();
    return res;
}
void la_parser_set(struct la_parser *res, const struct la_parser *a) {
    la_parser_clear(res);
    res->str_size = a->str_size;
    res->data = malloc(res->str_size);
    memcpy(res->data, a->data, a->str_size);
}
void la_parser_clear(struct la_parser *res) {
    if (res->data != NULL) free(res->data);
    res->data = NULL;
    res->position = res->str_size = 0;

    res->current_line = 0;
    res->scope_pos = 0;
    array_clear(res->list);
}
void la_parser_free(struct la_parser *res) {
    if (res->data != NULL) free(res->data);
    array_free(res->list);
    free(res);
}

void la_parser_set_str(struct la_parser *res, char *data, size_t size) {
    la_parser_clear(res);
    res->str_size = size;
    res->data = malloc(res->str_size);
    memcpy(res->data, data, size);
}
void la_parser_set_file(struct la_parser *res, char *file_path) {
    la_parser_clear(res);

    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) return;
    fseek(fp, 0, SEEK_END);
    res->str_size = ftell(fp);

    fseek(fp, 0, SEEK_SET);
    res->data = malloc(res->str_size);
    for (size_t i = 0; i < res->str_size; i++) {
        res->data[i] = (char) getc(fp);
    }
    fclose(fp);
}