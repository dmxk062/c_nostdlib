#include "mmap.h"

void* mmap(u64 addr, u64 len, u64 prot, u64 flags, u64 fd, u64 offset) {
    return syscall5(SYS_MMAP, 
            (void*) addr,
            (void*) len,
            (void*)prot,
            (void*)flags,
            (void*)offset);
}
