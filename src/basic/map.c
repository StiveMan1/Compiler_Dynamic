#include "basic.h"

struct object_type map_type = {MAP_OP};
// Standard operations

struct map_st *map_new() {
    struct map_st *res = malloc(MAP_SIZE);

    res->data = NULL;
    res->name = NULL;
    res->size = 0;
    for (int i = 0; i < 64; i++) res->next[i] = NULL;
    return res;
}
void map_set(struct map_st *res, const struct map_st *a) {
    map_clear(res);
    res->size = a->size;
    res->name = malloc(a->size);
    memcpy(res->name, a->name, a->size);
    res->data = object_copy(a->data);
    for (int i = 0; i < 64; i++) {
        if (a->next[i] != NULL) res->next[i] = object_copy(a->next[i]);
    }
}
void map_clear(struct map_st *res) {
    if (res->name != NULL) free(res->name);
    res->name = NULL;
    if (res->data != NULL) object_free(res->data);
    res->data = NULL;
    for (int i = 0; i < 64; i++) {
        if (res->next[i] != NULL) object_free(res->next[i]);
        res->next[i] = NULL;
    }
}
void map_free(struct map_st *res) {
    if (res->data != NULL) object_free(res->data);
    if (res->name != NULL) free(res->name);
    for (int i = 0; i < 64; i++) {
        if (res->next[i] != NULL) object_free(res->next[i]);
    }
    free(res);
}
int map_cmp(const struct map_st *obj1, const struct map_st *obj2) {
    if (obj1 == obj2) return 0;
    return 2;
}

unsigned short set_char_64(char x) {
    if (x >= '0' && x <= '9') {
        return (unsigned short) (x - '0');
    }
    if (x >= 'a' && x <= 'z') {
        return (unsigned short) (x - 'a' + 10);
    }
    if (x >= 'A' && x <= 'Z') {
        return (unsigned short) (x - 'A' + 36);
    }
    if (x == '-') {
        return (unsigned short) 62;
    }
    if (x == '_') {
        return (unsigned short) 63;
    }
    return (unsigned short) 0;
}

// Class Methods
void map_set_name_(struct map_st *res, const char *name, size_t size) {
    if (res->name != NULL) free(res->name);
    res->size = size;
    if (size != 0) {
        res->name = malloc(res->size);
        memcpy(res->name, name, size);
    } else {
        res->name = NULL;
    }
}
void map_clear_(struct map_st *res) {
    if (res->data != NULL) object_free(res->data);
    res->data = NULL;
    for (int i = 0; i < 64; i++) {
        if (res->next[i] != NULL) object_free(res->next[i]);
        res->next[i] = NULL;
    }
}
struct object_st *map_set_elm(struct map_st *res, char *name, size_t size) {
    while (1) {
        if (size < res->size || memcmp(name, res->name, res->size) != 0) {
            size_t i = 0;
            size_t S = res->size;
            if (size < S) S = size;
            for (; i < S; i++) {
                if (name[i] != res->name[i]) break;
            }
            struct object_st *obj = object_new();
            if (size == i) {
                struct object_st *temp_obj = object_new();
                object_set_type(temp_obj, MAP_TYPE);
                struct map_st *temp_tree = temp_obj->data;
                map_set(temp_tree, res);

                map_clear_(res);
                res->next[set_char_64(res->name[size])] = temp_obj;

                map_set_name_(temp_tree, &res->name[size + 1], res->size - size - 1);
                map_set_name_(res, name, size);

                res->data = obj;
            } else {
                struct object_st *temp_obj = object_new();
                object_set_type(temp_obj, MAP_TYPE);
                struct map_st *temp_tree = temp_obj->data;
                map_set(temp_tree, res);

                map_clear_(res);
                res->next[set_char_64(res->name[i])] = temp_obj;
                res->next[set_char_64(name[i])] = object_new();
                object_set_type(res->next[set_char_64(name[i])], MAP_TYPE);

                map_set_name_(temp_tree, &res->name[i + 1], res->size - i - 1);
                map_set_name_(res, name, i);
                map_set_name_(res->next[set_char_64(name[i])]->data, &name[i + 1], size - i - 1);

                ((struct map_st *)res->next[set_char_64(name[i])]->data)->data = obj;
            }
            return object_copy(obj);
        }
        if (size == res->size) {
            if (res->data == NULL) res->data = object_new();
            return object_copy(res->data);
        }
        int char_ = set_char_64(name[res->size]);
        if (res->next[char_] == NULL) {
            res->next[char_] = object_new();
            object_set_type(res->next[char_], MAP_TYPE);
            struct map_st *temp_tree = res->next[char_]->data;

            map_set_name_(temp_tree, &name[res->size + 1], size - res->size - 1);
            temp_tree->data = object_new();
            return object_copy(temp_tree->data);
        }
        size_t temp_size = res->size;
        res = res->next[char_]->data;
        name = &name[temp_size + 1];
        size = size - temp_size - 1;
    }
}
struct object_st *map_get_elm(struct map_st *res, char *name, size_t size) {
    while (1) {
        if (size < res->size || memcmp(name, res->name, res->size) != 0) {
            return NULL;
        }
        if (size == res->size) return object_copy(res->data);
        int char_ = set_char_64(name[res->size]);
        if (res->next[char_] == NULL) return NULL;
        size_t temp_size = res->size;
        res = res->next[char_]->data;
        name = &name[temp_size + 1];
        size = size - temp_size - 1;
    }
}