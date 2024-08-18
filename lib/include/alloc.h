#pragma once
#ifndef _ALLOC_H
#define _ALLOC_H

address malloc(u64 size);
void    free(address ptr);

u64 Alloc_get_in_use();
u64 Alloc_get_num_pages();

#endif
