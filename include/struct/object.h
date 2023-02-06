#ifndef OBJECT_H
#define OBJECT_H

// Container of Standard operations
#define METHOD_NEW        (void *(*)())
#define METHOD_SET        (void (*)(void *, const void *))
#define METHOD_CLEAR      (void (*)(void *))
#define METHOD_FREE       (void (*)(void *))
#define METHOD_CMP        (int (*)(const void *, const void *))

#define METHOD_GET_TLV    (void (*)(const void *, struct string_st *))
#define METHOD_SET_TLV    (void (*)(void *, const struct string_st *))

#define METHOD_MATH       (void (*)(struct object_st *, const void *, const struct object_st *))
#define METHOD_CONVERT    (void (*)(struct object_st *, const void *))

struct string_st;
struct object_st;

struct object_op {
    void *(*_new)();
    void (*_set)(void *, const void *);
    void (*_clear)(void *);
    void (*_free)(void *);
    int (*_cmp)(const void *, const void *);
};

struct object_tlv {
    void (*_get_tlv)(const void *, struct string_st *);
    void (*_set_tlv)(void *, const struct string_st *);
};

struct object_type {
    struct object_op self;
};

// Object Class
struct object_st {
    struct object_type *type;
    short counter;
    void *data;
};
// Standard operations
struct object_st *object_new();
void object_set(struct object_st *, const struct object_st *);
void object_clear(struct object_st *);
void object_free(struct object_st *);
int object_cmp(const struct object_st *, const struct object_st *);

// Pointer methods
struct object_st *object_copy(struct object_st *);
void object_set_type(struct object_st *, struct object_type *);

// Print
void print_obj(const struct object_st *res, int tabs);

#endif //OBJECT_H
