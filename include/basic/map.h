#ifndef MAP_H
#define MAP_H

#include "basic.h"

// Map Class
struct map_st{
    struct string_st **names;
    struct object_st **datas;

    size_t size, mx_size;
};
// Standard operations
struct map_st *map_new();
void map_set(struct map_st *, const struct map_st *);
void map_clear(struct map_st *);
void map_free(struct map_st *);
int map_cmp(const struct map_st *, const struct map_st *);
int map__cmp(struct error_st *, struct map_st *, const struct object_st *);

// Class Methods
void map_resize(struct map_st *, size_t );
struct object_st *map_set_elm(struct map_st *, const struct string_st *);
struct object_st *map_get_elm(struct map_st *, const struct string_st *);
struct object_st *map_set_elm_int(struct map_st *, const struct integer_st *);

// Math Methods
void map__add(struct object_st *, struct error_st *, const struct map_st *, const struct object_st *);

// Convert Methods
void map__str(struct object_st *, struct error_st *, struct map_st *);

// Sub method
struct object_st *map_subscript(struct error_st *, struct map_st *, const struct object_st *);
struct object_st *map_attrib(struct error_st *, struct map_st *, const struct object_st *);

void print_map(const struct map_st *, int tabs);

#endif //MAP_H
