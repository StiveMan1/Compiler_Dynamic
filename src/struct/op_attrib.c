#include "struct.h"

struct object_type op_attrib_type = {OP_ATTRIB_OP};


struct op_attrib *op_attrib_new() {
    struct op_attrib *res = malloc(OP_ATTRIB_SIZE);
    res->name = string_new();
    res->data = NULL;
    return res;
}
void op_attrib_set(struct op_attrib *res, const struct op_attrib *a) {
    string_set(res->name, a->name);
    if (res->data != NULL) object_free(res->data);
    res->data = object_copy(res->data);
}
void op_attrib_clear(struct op_attrib *res) {
    string_clear(res->name);
    if (res->data != NULL) object_free(res->data);
    res->data = NULL;
}
void op_attrib_free(struct op_attrib *res) {
    string_free(res->name);
    if (res->data != NULL) object_free(res->data);
    free(res);
}

void op_attrib_set_name(struct op_attrib *res, struct string_st *name) {
    string_set(res->name, name);
}
void op_attrib_new_data(struct op_attrib *res) {
    if (res->data != NULL) object_free(res->data);
    res->data = object_new();
}
int op_attrib_cmp_name(struct op_attrib *res, struct string_st *name) {
    return string_cmp(res->name, name);
}
struct object_st *op_attrib_get_data(struct op_attrib *res) {
    return res->data;
}