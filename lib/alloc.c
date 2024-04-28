#include <alloc.h>
#include <mmap.h>

/*
 * Memory Allocator
 * Based on a linked list design that uses mmap() 
 */

// the start of our linked list
mem_chunk* global_head = NULL;
// whatever element is at the end
mem_chunk* global_last = NULL;

// return the first chunk that is large enough
static
mem_chunk* mem_chunk_find(mem_chunk* head, u64 size) {
    mem_chunk* ptr = head;
    while (ptr != NULL) {
        if (ptr->free == TRUE && ptr->size >= (size + MEM_STRUCT_SIZE)) {
            return ptr;
        }
        global_last = ptr;
        ptr = ptr->next;
    }
    return ptr;
}


// get a new page from linux
static
mem_chunk* alloc_new_page(u64 size) {
    RESULT(untyped) memspace = mmap(0,
            size,
            PROT_READ|PROT_WRITE,
            MAP_ANONYMOUS|MAP_PRIVATE,
            0, 0);
    if (!memspace.success) {
        return NULL;
    }
    mem_chunk* ptr = memspace.value;
    ptr->size = MEM_PAGE_SIZE - MEM_STRUCT_SIZE;
    ptr->prev = NULL;
    ptr->next = NULL;
    ptr->free = TRUE;

    return ptr;
}

// get the next fitting page size that is larger than what we are given
static
u64 get_page_size(u64 size) {
    return ((size + MEM_PAGE_SIZE - 1) / MEM_PAGE_SIZE) * MEM_PAGE_SIZE;
}

static
void mem_chunk_split(mem_chunk* ptr, u64 size) {
    mem_chunk* new;

    new = (ptr->end) + size;

    new->size = ptr->size - size - MEM_STRUCT_SIZE;
    new->free = TRUE;
    new->next = ptr->next;
    new->prev = ptr;

    if ((new->next) != NULL) {
        (new->next)->prev = new;
    }

    ptr->size = size;
    ptr->free = FALSE;
    ptr->next = new;
}

void* malloc(u64 size) {

    // first run, allocate a page for our first object
    if (global_head == NULL) {
        u64 new_size = size;
        
        mem_chunk* new = alloc_new_page(get_page_size(size));
        // oops, either too big of a request or we are oom
        if (new == NULL) {
            return NULL;
        }
        // new head
        global_head = new;
        // unlikely that the user asks for exactly 4096
        mem_chunk_split(new, size);

        return new->end;
    }
    
    mem_chunk* ptr;
    // try to find a block in the area we already mapped
    ptr = mem_chunk_find(global_head, size);

    // we didnt find a large enough segment
    if (ptr == NULL) {

        // get a new page
        mem_chunk* new = alloc_new_page(get_page_size(size));
        if (new == NULL) {
            return NULL;
        }

        // same as in the initial setup
        mem_chunk_split(new, size);
        global_last->next = new;
        new->prev = global_last;

        return new->end;
    }
    // otherwise just return the segment we need
    mem_chunk_split(ptr, size);
    return ptr->end;
}


static
void merge_prev(mem_chunk* freed) {
    mem_chunk* prev;
    prev = freed->prev;

    if (prev != NULL && prev->free == TRUE) {
        prev->size = prev->size + freed->size + MEM_STRUCT_SIZE;
        prev->next = freed->next;
        if ((freed->next) != NULL) {
            (freed->next)->next = prev;
        }
    }
}

static
void merge_next(mem_chunk* freed) {
    mem_chunk* next;
    next = freed->next;


    if (next != NULL && next->free == TRUE) {
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
    mem_chunk* target;
    target = ptr - MEM_STRUCT_SIZE + sizeof(char*);
    target->free = TRUE;

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
}

u64 get_used_chunk_count() {
    u64 count = 0;
    mem_chunk* ptr = global_head;
    while (ptr != NULL) {
        if (!ptr->free)
            count++;
        ptr = ptr->next;
    }

    return count;
}
