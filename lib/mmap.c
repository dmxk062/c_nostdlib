#include <mmap.h>
#include <syscall.h>

Result(untyped) mmap(u64 addr, u64 len, u64 prot, u64 flags, u64 fd, u64 offset) {
    i64 ret = (i64) syscall5(SYS_MMAP, 
            (void*) addr,
            (void*) len,
            (void*)prot,
            (void*)flags,
            (void*)offset);
    if (ret < 0) 
        return Err(untyped, -ret);
    else
        return Ok(untyped, (untyped)ret);
}
