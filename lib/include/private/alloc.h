#pragma once
#ifndef _ALLOC_PRIVATE_H
#define _ALLOC_PRIVATE_H
#include <types.h>

#define ALLOC_PAGE_MIN_SIZE (1024 * 1024 * 8) // allocate at least 4mb
#define ALLOC_SPLIT_THRESHOLD 16

/*
 * An individual allocation
 */
typedef struct AllocChunk {
    /* Size for the allocation */
    u64 usable_size;
    /* Used or not? */
    bool free;
    /* Tag used to check if smth is up to date, a tag of 0 is considered to be empty */
    u64 tag;

    struct AllocChunk* next;
    struct AllocChunk* prev;
} AllocChunk;

/*
 * A memory mapped page
 */
typedef struct AllocPage {
    /* Total size of allocation, only used for unmapping */
    u64 size;
    /* Size of usable space */
    u64 usable_size;
    /* Number of allocations in page */
    u64 num_chunks;

    /* Start of usable area, used for `free()` */
    address start;
    /* End of usable area, used for `free()` */
    address end;

    /* Linked list pointers */
    struct AllocPage* next;
    struct AllocPage* prev;
} AllocPage;

/*
 * Global structure for the allocator 
 */
typedef struct {
    /* */
    bool initialized;
    /* Number of mapped pages */
    u64 num_pages;
    /* Number of used chunks */
    u64 num_allocs;

    AllocPage* first;
    AllocPage* last;
} AllocHead;
#endif
