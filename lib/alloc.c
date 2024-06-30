#include "types.h"
#include <alloc.h>
#include <mmap.h>

/*
 * Memory Allocator
 * Based on a linked list design that uses mmap() 
 */

// the start of our linked list
static MallocChunk* Malloc_global_head = NULL;
// whatever element is at the end
static MallocChunk* Malloc_global_last = NULL;

static struct {
    u64 pages;
    u64 allocated;
    u64 in_use;

} malloc_stats;

// return the first chunk that is large enough
static
MallocChunk* mem_chunk_find(MallocChunk* head, u64 size) {
    MallocChunk* ptr = head;
    while (ptr != NULL) {
        if (ptr->free == true && ptr->size >= (size + MEM_STRUCT_SIZE)) {
            return ptr;
        }
        Malloc_global_last = ptr;
        ptr = ptr->next;
    }
    return ptr;
}


// get a new page from linux
static
MallocChunk* alloc_new_page(u64 size) {
    Result(untyped) memspace = mmap(0,
            size,
            PROT_READ|PROT_WRITE,
            MAP_ANONYMOUS|MAP_PRIVATE,
            0, 0);
    if (!memspace.ok) {
        return NULL;
    }
    MallocChunk* ptr = memspace.value;
    ptr->size = MEM_PAGE_SIZE - MEM_STRUCT_SIZE;
    ptr->prev = NULL;
    ptr->next = NULL;
    ptr->free = true;
    malloc_stats.pages++;

    return ptr;
}

// get the next fitting page size that is larger than what we are given
static inline
u64 get_page_size(u64 size) {
    return ((size + MEM_PAGE_SIZE - 1) / MEM_PAGE_SIZE) * MEM_PAGE_SIZE;
}

static
void mem_chunk_split(MallocChunk* ptr, u64 size) {

    char* _new = (ptr->end) + size;
    MallocChunk* new = (MallocChunk*)_new;

    new->size = ptr->size - size - MEM_STRUCT_SIZE;
    new->free = true;
    new->next = ptr->next;
    new->prev = ptr;

    if ((new->next) != NULL) {
        (new->next)->prev = new;
    }

    ptr->size = size;
    ptr->free = false;
    ptr->next = new;
}

void* malloc(u64 size) {

    // first run, allocate a page for our first object
    if (Malloc_global_head == NULL) {
        // u64 new_size = size;
        malloc_stats.pages  = 0;
        malloc_stats.in_use = 0;
        malloc_stats.allocated  = 0;
        
        MallocChunk* new = alloc_new_page(get_page_size(size));
        // oops, either too big of a request or we are oom
        if (new == NULL) {
            return NULL;
        }
        // new head
        Malloc_global_head = new;
        // unlikely that the user asks for exactly 4096
        mem_chunk_split(new, size);

        malloc_stats.allocated++;
        malloc_stats.in_use++;
        return new->end;
    }
    
    MallocChunk* ptr;
    // try to find a block in the area we already mapped
    ptr = mem_chunk_find(Malloc_global_head, size);

    // we didnt find a large enough segment
    if (ptr == NULL) {

        // get a new page
        MallocChunk* new = alloc_new_page(get_page_size(size));
        if (new == NULL) {
            return NULL;
        }

        // same as in the initial setup
        mem_chunk_split(new, size);
        Malloc_global_last->next = new;
        new->prev = Malloc_global_last;

        malloc_stats.allocated++;
        malloc_stats.in_use++;
        return new->end;
    }
    // otherwise just return the segment we need
    mem_chunk_split(ptr, size);

    malloc_stats.allocated++;
    malloc_stats.in_use++;
    return ptr->end;
}


static
void merge_prev(MallocChunk* freed) {
    MallocChunk* prev;
    prev = freed->prev;

    if (prev != NULL && prev->free == true) {
        prev->size = prev->size + freed->size + MEM_STRUCT_SIZE;
        prev->next = freed->next;
        if ((freed->next) != NULL) {
            (freed->next)->next = prev;
        }
    }
}

static
void merge_next(MallocChunk* freed) {
    MallocChunk* next;
    next = freed->next;


    if (next != NULL && next->free == true) {
        freed->size = freed->size + MEM_STRUCT_SIZE + next->size;
        freed->next = next->next;

        if ((next->next) != NULL) {
            (next->next)->prev = freed;
        }

    }
}

void free(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    /*
     * FIXME: find some way to check if it was really a page we allocated
     */
    MallocChunk* target;
    target = ptr - MEM_STRUCT_SIZE + sizeof(char*);
    target->free = true;

    /*
     * This is a bit hacky
     * we need to check if the blocks are on the same page we mmaped
     * otherwise merging them would leave gaps
     * so we calculate that using pointer offsets
     */
    if (target->next != NULL && target->next == (target->end + target->size)){
        merge_next(target);
    }
    if (target->prev != NULL && target == ((target->prev)->end) + (target->prev)->size) {
        merge_prev(target);
    }
    malloc_stats.in_use--;
}

u64 Malloc_get_used_count() {
    return malloc_stats.in_use;
}
u64 Malloc_get_page_count() {
    return malloc_stats.pages;
}

u64 Malloc_get_allocated_count() {
    return malloc_stats.allocated;
}
