#include "struct.h"
#include <sys/time.h>

struct object_type integer_type = {INTEGER_OP, &integer_tlv, &integer_math, &integer_convert};
// Standard operations
struct integer_st *integer_new() {
    struct integer_st *res = malloc(INTEGER_SIZE);
    res->num = 0;
    return res;
}
void integer_set(struct integer_st *res, const struct integer_st *a) {
    if (integer_is_null(a)) return integer_clear(res);
    res->num = a->num;
}
void integer_clear(struct integer_st *res) {
    res->num = 0;
}
void integer_free(struct integer_st *res) {
    free(res);
}
int integer_cmp(const struct integer_st *obj1, const struct integer_st *obj2) {
    if(obj1->num < obj2->num) return -1;
    if(obj1->num > obj2->num) return 1;
    return 0;
}

void _integer_set_str_dec(struct integer_st *res, const char *str, size_t size);
void _integer_set_str_hex(struct integer_st *res, const char *str, size_t size);
void _integer_set_str_oct(struct integer_st *res, const char *str, size_t size);
void _integer_set_str_bin(struct integer_st *res, const char *str, size_t size);

// Cmp methods
int integer_is_null(const struct integer_st *res) {
    if (res == NULL || res->num == 0) return 1;
}

