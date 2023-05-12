#include "memory/memory.h"

struct mem_ctx mem_ctx = {NULL, 0};


#define MEM_PTR(ptr) (*(void **) (ptr))

#define POOL_SIZE 4096
#define POOL_NUMBER 64

#define POOL_FREE   0x00
#define POOL_USED   0x01
#define POOL_FULL   0x02

#define POOL_INIT (struct mem_pool) {0, 0, 0, NULL}

// Memory pool
struct mem_pool {
    char pool_flag;
    unsigned short pool_size;
    unsigned short pool_filled;
    void *first_free;
};

void *pool_malloc(struct mem_pool *pool, size_t size, void *pos) {
    void *res = pool->first_free;
//    if(size == 8) printf("C %p : %p %d\n", pool, pool->first_free, pool->pool_filled);
    if (res != NULL) pool->first_free = MEM_PTR(pool->first_free);
    else res = pos + (pool->pool_size * pool->pool_filled);

    memset(res, 0, size);

    pool->pool_size = size;
    pool->pool_flag = (++pool->pool_filled == POOL_SIZE / pool->pool_size) ? POOL_FULL : POOL_USED;
//    if(size == 8) printf("C %p : %p %p %d\n", pool, pool->first_free, res, pool->pool_filled);

    return res;
}

void pool_free(struct mem_pool *pool, void *data) {
//    if(pool->pool_size == 8) printf("D %p : %p %p %d\n", pool, pool->first_free, data, pool->pool_filled);
    MEM_PTR(data) = pool->first_free;
    pool->first_free = data;

    pool->pool_flag = (--pool->pool_filled == 0) ? POOL_FREE : POOL_USED;
    if (POOL_FREE == pool->pool_flag) pool->first_free = NULL;
//    if(pool->pool_size == 8) printf("D %p : %p %p %d\n", pool, pool->first_free, data, pool->pool_filled);
}


#define ARENA_CUR if (cur_arena->ctx <= data && data < cur_arena->ctx + POOL_NUMBER * POOL_SIZE)

struct mem_arena {
    void *ctx;
    struct mem_arena *left;
    struct mem_pool pools[POOL_NUMBER];
};

// Create 
struct mem_arena *arena_new() {
    struct mem_arena *res = malloc(sizeof(struct mem_arena));
    res->ctx = malloc(POOL_SIZE * POOL_NUMBER);

    res->left = NULL;
    for (int i = 0; i < POOL_NUMBER; i++) res->pools[i] = POOL_INIT;

    return res;
}

// Malloc arena
void *arena_malloc(struct mem_arena *arena, size_t size) {
    int pool_id = -1;
    for (int i = 0; i < POOL_NUMBER; i++) {
        if (arena->pools[i].pool_flag == POOL_USED && arena->pools[i].pool_size == size &&
            (pool_id == -1 || arena->pools[pool_id].pool_filled < arena->pools[i].pool_filled))
            pool_id = i;
        if (arena->pools[i].pool_flag == POOL_FREE && pool_id == -1) pool_id = i;
    }
    if (pool_id == -1) {
        return NULL;
    }
    void *res = pool_malloc(&arena->pools[pool_id], size, arena->ctx + POOL_SIZE * pool_id);
    return res;
}


#define CTX_LOOP for (struct mem_arena *cur_arena = ctx->first_arena; cur_arena != NULL; cur_arena = cur_arena->left)

// Malloc SKR
void *SKR_malloc(struct mem_ctx *ctx, size_t size) {
    if (size == 0) return NULL;
    unsigned short need_size = (size / 8 + (size % 8 != 0)) * 8;
    ctx->filled++; // DEBUG

    void *res = NULL;
    CTX_LOOP {
        res = arena_malloc(cur_arena, need_size);
        if (res != NULL) return res;
    }

    struct mem_arena *arena = arena_new();
    arena->left = ctx->first_arena;
    ctx->first_arena = arena;

    return arena_malloc(ctx->first_arena, need_size);
}

// Realloc SKR
void *SKR_realloc(struct mem_ctx *ctx, void *data, size_t size) {
    ctx->filled--; // DEBUG
    void *res = SKR_malloc(ctx, size);

    CTX_LOOP {
        ARENA_CUR {
            memcpy(res, data, cur_arena->pools[(data - cur_arena->ctx) / POOL_SIZE].pool_size);
            pool_free(&cur_arena->pools[(data - cur_arena->ctx) / POOL_SIZE], data);
            return res;
        }
    }
    return res;
}

// Free SKR
void SKR_free(struct mem_ctx *ctx, void *data) {
    ctx->filled--; // DEBUG

    CTX_LOOP {
        ARENA_CUR {
            return pool_free(&cur_arena->pools[(data - cur_arena->ctx) / POOL_SIZE], data);
        }
    }
}

