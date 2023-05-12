#include "basic.h"

struct object_math_op map_math = {NULL, NULL, NULL, METHOD_MATH &map__add};
struct object_convert map_convert = {NULL, NULL, NULL, METHOD_CONVERT &map__str};
struct object_sub map_sub = {METHOD_SUBSCRIPT &map_subscript, METHOD_ATTRIB &map_attrib};
struct object_type map_type = {MAP_OP, &map_sub, &map_convert, &map_math};
// Standard operations
// Create
struct map_st *map_new() {
    struct map_st *res = Malloc(MAP_SIZE);
    res->names = NULL;
    res->datas = NULL;

    res->size = 0;
    res->mx_size = 0;
    return res;
}
// Set value
void map_set(struct map_st *res, const struct map_st *a) {
    map_clear(res);
    map_resize(res, a->size);
    for (size_t i = 0; i < a->size; i ++) {
        if (a->names[i] != NULL) {
            res->names[i] = string_new();
            string_set(res->names[i], a->names[i]);
        }
        if (a->datas[i] != NULL)
            res->datas[i] = object_copy(a->datas[i]);
    }
}
// Clear
void map_clear(struct map_st *res) {
    map_resize(res, 0);
}
// Free
void map_free(struct map_st *res) {
    map_resize(res, 0);
    if (res->names != NULL) Free(res->names);
    if (res->datas != NULL) Free(res->datas);
    Free(res);
}
// Cmp
int map_cmp(const struct map_st *obj1, const struct map_st *obj2) {
    if (obj1->size != obj2->size) return 2;
    size_t counter = 0;
    struct error_st err;
    for (size_t i = 0; i < obj1->size; i++) {
        for (size_t j = 0; j < obj2->size; j++) {
            if (string_cmp(obj1->names[i], obj2->names[i]) == 0) {
                if (object_cmp(&err, obj1->datas[i], obj2->datas[i]) != 0) return 2;
                counter ++;
            }
        }
    }
    if (counter == obj2->size) return 0;
    return 2;
}
int map__cmp(struct error_st *err, struct map_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    if(obj2->type != MAP_TYPE) {
        err->present = 1;
        string_set_str(err->type, INTERPRETER_ERROR, 17);
        string_set_str(err->message, "map can't compare with non map type", 39);
        return 2;
    }
    struct map_st *map = obj2->data;
    if (obj1->size != map->size) return 2;
    size_t counter = 0;
    for (size_t i = 0; i < obj1->size; i++) {
        for (size_t j = 0; j < map->size; j++) {
            if (string_cmp(obj1->names[i], map->names[i]) == 0) {
                if (object_cmp(err, obj1->datas[i], map->datas[i]) != 0) return 2;
                counter ++;
            }
        }
    }
    if (counter == obj1->size) return 0;
    return 2;
}
// Checking type of char
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
// Change size of the map
void map_resize(struct map_st *res, size_t size) {
    if (res->datas == NULL && size != 0) {
        res->names = Malloc(sizeof (struct string_st *) * size);
        res->datas = Malloc(sizeof (struct object_st *) * size);
        res->mx_size = size;
        for (size_t i = 0; i < res->mx_size; i++) {
            res->names[i] = NULL;
            res->datas[i] = NULL;
        }
    } else if (size > res->mx_size) {
        res->names = Realloc(res->names, sizeof (struct string_st *) * size * 2);
        res->datas = Realloc(res->datas, sizeof (struct object_st *) * size * 2);
        for (size_t i = res->mx_size; i < size * 2; i ++) {
            res->names[i] = NULL;
            res->datas[i] = NULL;
        }
        res->mx_size = size * 2;
    }
    if (size < res->size) {
        for (size_t i = size; i < res->mx_size; i ++) {
            if (res->names[i] != NULL) string_free(res->names[i]);
            if (res->datas[i] != NULL) object_free(res->datas[i]);
            res->names[i] = NULL;
            res->datas[i] = NULL;
        }
    }
    res->size = size;
}
// Setter
struct object_st *map_set_elm(struct map_st *res, const struct string_st *str) {
    for (size_t i = 0; i < res->size; i++) {
        if (res->names[i] != NULL && string_cmp(res->names[i], str) == 0) return object_copy(res->datas[i]);
    }
    map_resize(res, res->size + 1);
    res->names[res->size - 1] = string_new();
    string_set(res->names[res->size - 1], str);
    return object_copy(res->datas[res->size - 1] = object_new());
}
// Getter
struct object_st *map_get_elm(struct map_st *res, const struct string_st *str) {
    for (size_t i = 0; i < res->size; i++) {
        if (res->names[i] != NULL && string_cmp(res->names[i], str) == 0) return object_copy(res->datas[i]);
    }
    return NULL;
}
// Set element 
struct object_st *map_set_elm_int(struct map_st *res, const struct integer_st *int_) {
    if (int_->data >= res->size) map_resize(res, int_->data + 1);
    if (res->names[int_->data] == NULL) {
        res->names[int_->data] = string_new();
        res->datas[int_->data] = object_new();    }
    return object_copy(res->datas[int_->data]);
}

// Math Methods
// Addition
void map__add(struct object_st *res, struct error_st *err, const struct map_st *obj1, const struct object_st *obj2) {
    while (obj2 != NULL && obj2->type == OBJECT_TYPE) obj2 = obj2->data;
    if (obj2->type != MAP_TYPE) {
        err->present = 1;
        string_set_str(err->type, INTERPRETER_ERROR, 17);
        string_set_str(err->message, "map can use addition only with map", 34);
        return;
    }
    object_set_type(res, MAP_TYPE);
    struct map_st *res_map = res->data;
    struct map_st *map_obj2 = obj2->data;

    map_set(res_map, obj1);
    for (size_t i = 0; i < map_obj2->size; i++) {
        int ok = 1;
        for (size_t j = 0; j < res_map->size; j ++) {
            if (string_cmp(res_map->names[j], map_obj2->names[i]) == 0) {
                ok = 0;
                break;
            }
        }
        if (ok) {
            map_resize(res_map, res_map->size + 1);
            res_map->names[res_map->size - 1] = string_new();
            string_set(res_map->names[res_map->size - 1], map_obj2->names[i]);
            res_map->datas[res_map->size - 1] = object_copy(map_obj2->datas[i]);
        }
    }
}

// Convert Methods
// To string
void map__str(struct object_st *res, struct error_st *err, struct map_st *obj) {
    object_set_type(res, STRING_TYPE);
    string_set_str(res->data, "{", 1);
    struct object_st *temp = object_new();
    object_set_type(temp, STRING_TYPE);
    for(size_t i = 0; i < obj->size; i++){
        if (obj->names[i] != NULL) {
            string_concat(res->data, obj->names[i]);
            if (obj->datas[i] != NULL && obj->datas[i]->type != NONE_TYPE) {
                string_set_str(temp->data, " := ", 4);
                string_concat(res->data, temp->data);
                object__str(temp, err, obj->datas[i]);
                if (err->present) {
                    object_free(temp);
                    return;
                }
                string_concat(res->data, temp->data);
            }
            if (i + 1 < obj->size) {
                string_set_str(temp->data, ", ", 2);
                string_concat(res->data, temp->data);
            }
        }
    }
    string_set_str(temp->data, "}", 1);
    string_concat(res->data, temp->data);
    object_free(temp);
}

// Sub method
struct object_st *map_subscript(struct error_st *err, struct map_st *map, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj->type == INTEGER_TYPE) {
        return map_set_elm_int(map, obj->data);
    }
    struct object_st *temp = object_new();
    object__int(temp, err, obj);
    if(err->present) {
        object_free(temp);
        return NULL;
    }
    struct object_st *result = map_set_elm(map, obj->data);
    object_free(temp);
    return result;
}
struct object_st *map_attrib(struct error_st *err, struct map_st *map, const struct object_st *obj) {
    while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;
    if (obj->type == INTEGER_TYPE) {
        return map_set_elm_int(map, obj->data);
    }
    struct object_st *temp = object_new();
    object__str(temp, err, obj);
    if(err->present) {
        object_free(temp);
        return NULL;
    }
    struct object_st *result = map_set_elm(map, temp->data);
    object_free(temp);
    return result;
}