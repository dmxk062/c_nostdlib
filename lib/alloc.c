#include "io.h"
#include "types.h"
#include <alloc.h>
#include <alloc_private.h>
#include <mmap.h>
#include <errno.h>

static AllocHead pg_Head = {
    .initialized = false,
    .num_pages = 0,
    .num_allocs = 0,
    .first = NULL,
    .last = NULL,
};

static AllocPage* p_AllocPage_new(u64 size) {
    Result(address) mapped_page = mmap(NULL, size, PROT_READ | PROT_WRITE,
                                       MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
    /*
     * TODO: pass on error number
     * pretty much an unhandleable error anyways
     */
    if (!mapped_page.ok) {
        return NULL;
    }

    AllocPage* new_page = mapped_page.value;
    new_page->size = size;
    new_page->usable_size = (size - sizeof(AllocPage));
    new_page->start = (address)new_page + sizeof(AllocPage);
    new_page->end   = (address)new_page->start + new_page->usable_size;

    AllocChunk* initial_chunk = new_page->start;
    initial_chunk->free = true;
    initial_chunk->usable_size = new_page->usable_size - sizeof(AllocChunk);
    initial_chunk->next = NULL;
    initial_chunk->prev = NULL;

    new_page->num_chunks = 1;

    return new_page;
}

static errno_t p_init(u64 first_page_size) {
    AllocPage* initial_page = p_AllocPage_new(first_page_size);
    if (!initial_page) {
        return ENOMEM;
    }

    pg_Head.first = initial_page;
    pg_Head.last  = initial_page;
    pg_Head.num_pages++;

    pg_Head.initialized = true;
    return 0;
}

static u64 p_AllocPage_get_max_avail_size(AllocPage* page) {
    AllocChunk* chunk = page->start;
    u64 size = 0;
    while (chunk) {
        if (chunk->free && chunk->usable_size >= size) {
            size = chunk->usable_size;
        }
        chunk = chunk->next;
    } 
    return size;
}

static AllocPage* p_AllocPage_get_first_large_enough(u64 min_size) {
    AllocPage* page = pg_Head.first;
    while(page) {
        u64 size = p_AllocPage_get_max_avail_size(page);
        if (size >= min_size) {
            return page;
        }
        page = page->next;
    }
    return NULL;
}

static inline u64 p_get_alloc_page_size(u64 size) {
    u64 val =  ((size + sizeof(AllocPage) + sizeof(AllocChunk) + ALLOC_PAGE_MIN_SIZE - 1) / ALLOC_PAGE_MIN_SIZE) * ALLOC_PAGE_MIN_SIZE;
    return val;
}

static AllocChunk* p_AllocChunk_split(AllocPage* page, AllocChunk* chunk, u64 size) {
    /* Can't fit another allocation anyways */
    if (chunk->usable_size == size 
            || chunk->usable_size == size + sizeof(AllocChunk) 
            || chunk->usable_size < size + ALLOC_SPLIT_THRESHOLD) {
        return chunk;
    } 

    u64 alloc_size = size + sizeof(AllocChunk);
    AllocChunk* new_chunk = (address)chunk + alloc_size;
    new_chunk->prev = chunk;
    if (chunk->next) {
        (chunk->next)->prev = new_chunk;
    }
    new_chunk->next = chunk->next;
    chunk->next = new_chunk;
    new_chunk->usable_size = chunk->usable_size - alloc_size;
    chunk->usable_size = size;
    new_chunk->free = true;

    page->num_chunks++;
    return chunk;
}

static address p_AllocPage_alloc(AllocPage* page, u64 size) {
    AllocChunk* chunk = page->start;
    while (chunk) {
        if (chunk->free && chunk->usable_size >= size) {
            break;
        }
        chunk = chunk->next;
    }

    AllocChunk* new_chunk = p_AllocChunk_split(page, chunk, size);

    new_chunk->free = false;
    return (u8*)new_chunk + sizeof(AllocChunk);
}

address malloc(u64 size) {
    if (!pg_Head.initialized) {
        u64 target_size = p_get_alloc_page_size(size);
        errno_t err = p_init(target_size);
        if (err) {
            return NULL;
        }
    }

    AllocPage* page = p_AllocPage_get_first_large_enough(size);

    // Append a new page
    if (!page) {
        u64 target_size = p_get_alloc_page_size(size);
        AllocPage* new_page = p_AllocPage_new(target_size);
        if (!new_page) {
            return NULL;
        }

        pg_Head.last->next = new_page;
        new_page->prev = pg_Head.last;
        pg_Head.last = new_page;
        pg_Head.num_pages++;

        page = new_page;
    }

    pg_Head.num_allocs++;
    return p_AllocPage_alloc(page, size);
};

/*
 * Internal free representation
 * Return value:
 * 1: outside of any known page
 * 2: not directly on chunk
 */
static errno_t __free(address ptr) {
    /*
     * Walk all the pages we have to actually find the allocation and see if its within our range
     */ 
    AllocPage* page = pg_Head.first;
    while (page) {
        if (page->start < ptr && page->end > ptr) {
            break;
        }
        page = page->next;
    }
    if (!page) {
        return 1;
    }

    AllocChunk* chunk = page->start;
    while (chunk) {
        if (ptr == (u8*)chunk + sizeof(AllocChunk)) {
            break;
        }
        chunk = chunk->next;
    }

    if (!chunk) {
        return 2;
    }

    // free xD
    chunk->free = true;

    /*
     * Try to merge with adjacent ones
     * first prev then next
     */
    
    if (chunk->prev && (chunk->prev)->free) {
        AllocChunk* prev = chunk->prev;
        prev->usable_size += chunk->usable_size + sizeof(AllocChunk);
        prev->next = chunk->next;
        if (chunk->next) {
            (chunk->next)->prev = prev;
        }
    }

    if (chunk->next && (chunk->next)->free) {
        AllocChunk* next = chunk->next;
        chunk->usable_size += next->usable_size + sizeof(AllocChunk);
        chunk->next = next->next;
        if(next->next) {
            (next->next)->prev = chunk;
        }
    }

    pg_Head.num_allocs--;
    return 0;
}

void free(address ptr) {
    __free(ptr);
}

u64 Alloc_get_in_use() {
    return pg_Head.num_allocs;
}

u64 Alloc_get_num_pages() {
    return pg_Head.num_pages;
}
