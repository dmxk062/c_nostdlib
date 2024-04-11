#include "alloc.h"
#include "../lib/io.h"


mem_chunk* global_head = NULL;
mem_chunk* global_last = NULL;

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


mem_chunk* alloc_new_page(u64 size) {
    void* memspace = mmap(0,
            size,
            PROT_READ|PROT_WRITE,
            MAP_ANONYMOUS|MAP_PRIVATE,
            0, 0);
    mem_chunk* ptr = memspace;
    ptr->size = MEM_PAGE_SIZE - MEM_STRUCT_SIZE;
    ptr->prev = NULL;
    ptr->next = NULL;
    ptr->free = TRUE;

    return ptr;
}

u64 get_page_size(u64 size) {
    return ((size + MEM_PAGE_SIZE - 1) / MEM_PAGE_SIZE) * MEM_PAGE_SIZE;
}
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
    if (global_head == NULL) {
        u64 new_size = size;
        
        mem_chunk* new = alloc_new_page(get_page_size(size));
        global_head = new;
        mem_chunk_split(new, size);
        return new->end;
    }
    mem_chunk* ptr;
    ptr = mem_chunk_find(global_head, size);
    if (ptr == NULL) {
        mem_chunk* new = alloc_new_page(get_page_size(size));
        mem_chunk_split(new, size);
        global_last->next = new;
        new->prev = global_last;
        return new->end;
    }
    mem_chunk_split(ptr, size);
    return ptr->end;
}


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
    mem_chunk* target;
    target = ptr - MEM_STRUCT_SIZE + 8;
    target->free = TRUE;
    merge_prev(target);
    merge_next(target);
}
