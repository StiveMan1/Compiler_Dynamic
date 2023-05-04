#ifndef MEMORY_H
#define MEMORY_H

#include "stdlib.h"
#include <malloc.h>
#include <string.h>

struct mem_ctx {
    struct mem_arena *first_arena;
    long long filled;
};
extern struct mem_ctx mem_ctx;

void *SKR_malloc(struct mem_ctx *ctx, size_t size);
void *SKR_realloc(struct mem_ctx *ctx, void *data, size_t size);
void SKR_free(struct mem_ctx *ctx, void *data);

#define Malloc(size) SKR_malloc(&mem_ctx, size)
#define Realloc(data, size) SKR_realloc(&mem_ctx, data, size)
#define Free(data) SKR_free(&mem_ctx, data)

//#define Malloc(size) Malloc(size)
//#define Realloc(data, size) Realloc(data, size)
//#define Free(data) Free(data)

#endif //MEMORY_H
