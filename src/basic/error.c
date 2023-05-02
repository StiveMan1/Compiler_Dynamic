#include "basic.h"

// Standard operations
struct error_st *error_new() {
    struct error_st *res = malloc(ARRAY_SIZE);
    res->present = 0;
    res->type = NULL;
    res->message = NULL;
    res->line = 0;
    res->line_pos = 0;
    return res;
}
void error_set(struct error_st *res, const struct error_st *a) {
    if (error_is_null(res)) return;
    error_clear(res);
    if (error_is_null(a)) return;
    res->present = a->present;
    res->type = malloc(strlen(a->type));
    memcpy(res->type, a->type, strlen(a->type));
    res->message = malloc(strlen(a->message));
    memcpy(res->message, a->message, strlen(a->message));
    res->line = a->line;
    res->line_pos = a->line_pos;
}
void error_clear(struct error_st *res) {
    res->present = 0;
    if (res->type != NULL) free(res->type);
    if (res->message != NULL) free(res->message);
    res->type = NULL;
    res->message = NULL;
    res->line = 0;
    res->line_pos = 0;
}
void error_free(struct error_st *res) {
    if (res->type != NULL) free(res->type);
    if (res->message != NULL) free(res->message);
    free(res);
}
int error_cmp(const struct error_st *obj1, const struct error_st *obj2) {
    if (obj1->present > obj2->present) return 1;
    if (obj1->present < obj2->present) return -1;
    int res_cmp_sub = strcmp(obj1->type, obj2->type);
    if (res_cmp_sub == 0)
        res_cmp_sub = strcmp(obj1->message, obj2->message);
    return res_cmp_sub;
}
int error_is_null(const struct error_st *res) {
    return (res == NULL || res->present == 0);
}
