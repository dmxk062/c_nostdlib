#include "../include/types.h"
#include "../lib/syscall.h"

#define PROT_READ	1
#define PROT_WRITE	2
#define PROT_EXEC	4
#define PROT_NONE	0

#define MAP_TYPE	    0
#define MAP_SHARED	    1
#define MAP_PRIVATE	    2
#define MAP_ANONYMOUS   32



void* mmap(u64 addr, u64 len, u64 prot, u64 flags, u64 fd, u64 offset);
