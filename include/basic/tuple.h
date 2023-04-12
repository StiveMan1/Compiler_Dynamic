// Copyright 2020 Joshua J Baker. All rights reserved.
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file.

#ifndef TUPLEHASH_H
#define TUPLEHASH_H

#include "basic.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// tuplehash_st is an open addressed hash map using robinhood hashing.
struct tuplehash_st {
    struct object_st **data;
    size_t size, max_size;

    bool oom;
    size_t elsize;
    size_t cap;
    uint64_t seed0;
    uint64_t seed1;
    uint64_t (*hash)(const void *item, uint64_t seed0, uint64_t seed1);
    int (*compare)(const void *a, const void *b, void *udata);
    void (*elfree)(void *item);
    void *udata;
    size_t bucketsz;
    size_t nbuckets;
    size_t count;
    size_t mask;
    size_t growat;
    size_t shrinkat;
    void *buckets;
    void *spare;
    void *edata;
};

struct tuplehash_st *tuplehash_new(size_t elsize, size_t cap,
                                   uint64_t seed0, uint64_t seed1,
                                   uint64_t (*hash)(const void *item,
                                             uint64_t seed0, uint64_t seed1),
                                   int (*compare)(const void *a, const void *b,
                                           void *udata),
                                   void (*elfree)(void *item),
                                   void *udata);
struct tuplehash_st *tuplehash_new_with_allocator(
        size_t elsize, size_t cap,
        uint64_t seed0, uint64_t seed1,
        uint64_t (*hash)(const void *item,
                         uint64_t seed0, uint64_t seed1),
        int (*compare)(const void *a, const void *b,
                       void *udata),
        void (*elfree)(void *item),
        void *udata);
void tuplehash_free(struct tuplehash_st *map);
void tuplehash_clear(struct tuplehash_st *map, bool update_cap);
size_t tuplehash_count(struct tuplehash_st *map);
bool tuplehash_oom(struct tuplehash_st *map);
void *tuplehash_get(struct tuplehash_st *map, const void *item);
void *tuplehash_set(struct tuplehash_st *map, const void *item);
void *tuplehash_delete(struct tuplehash_st *map, void *item);
void *tuplehash_probe(struct tuplehash_st *map, uint64_t position);
bool tuplehash_scan(struct tuplehash_st *map,
                    bool (*iter)(const void *item, void *udata), void *udata);
bool tuplehash_iter(struct tuplehash_st *map, size_t *i, void **item);

uint64_t tuplehash_sip(const void *data, size_t len,
                       uint64_t seed0, uint64_t seed1);
uint64_t tuplehash_murmur(const void *data, size_t len,
                          uint64_t seed0, uint64_t seed1);

int tuplehash_is_null(const struct tuplehash_st *);

// Class methods
void tuplehash_resize(struct tuplehash_st *, size_t);

// Print
void print_tuplehash(const struct tuplehash_st *, int);

#endif //HASHMAP_H
