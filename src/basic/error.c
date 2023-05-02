#include "basic.h"

// Standard operations
struct error_st *error_new() {
    struct error_st *res = malloc(sizeof (struct error_st));
    res->present = 0;
    res->type = string_new();
    res->message = string_new();

    res->pos = 0;
    res->line_num = 0;
    res->line_pos = 0;
    return res;
}
void error_set(struct error_st *res, const struct error_st *a) {
    if (error_is_null(res)) return;
    error_clear(res);
    if (error_is_null(a)) return;

    res->present = a->present;
    string_set(res->type, a->type);
    string_set(res->message, a->message);

    res->pos = a->pos;
    res->line_num = res->line_num;
    res->line_pos = res->line_pos;
}
void error_clear(struct error_st *res) {
    if(res == NULL) return;
    res->present = 0;
    string_clear(res->type);
    string_clear(res->message);

    res->pos = 0;
    res->line_num = 0;
    res->line_pos = 0;
}
void error_free(struct error_st *res) {
    if(res == NULL) return;
    string_free(res->type);
    string_free(res->message);
    free(res);
}
int error_cmp(const struct error_st *obj1, const struct error_st *obj2) {
    if (obj1->present > obj2->present) return 1;
    if (obj1->present < obj2->present) return -1;
    int res_cmp_sub = string_cmp(obj1->type, obj2->type);
    if (res_cmp_sub == 0)
        res_cmp_sub = string_cmp(obj1->message, obj2->message);
    return res_cmp_sub;
}
int error_is_null(const struct error_st *res) {
    return (res == NULL || res->present == 0);
}

// Class methods
void error_fill_in(struct error_st *res, char* type, char* message, int pos, int line_num, int line_pos) {
    if(res == NULL) return;
    res->present = 1;
    string_set_str(res->type, type, strlen(type));
    string_set_str(res->message, message, strlen(message));
    res->pos = pos;
    res->line_num = line_num;
    res->line_pos = line_pos;
}