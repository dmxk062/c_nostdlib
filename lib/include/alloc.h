#pragma once
#include <types.h>

typedef struct mem_chunk {
    u64 size;
    bool free;

    struct mem_chunk* next;
    struct mem_chunk* prev;

    char end[1];
} mem_chunk;

#define MEM_STRUCT_SIZE sizeof(mem_chunk)

#define MEM_PAGE_SIZE 1024 * 2

void* malloc(u64 size);
void free(void* ptr);
