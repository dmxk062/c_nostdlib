#include "types.h"
#include <mmap.h>
#include <syscall.h>

Result(untyped) mmap(untyped addr, u64 len, u64 prot, u64 flags, u64 fd, u64 offset) {
    i64 ret = (i64)syscall6(SYS_MMAP, 
            (untyped)addr,
            (untyped)len,
            (untyped)prot,
            (untyped)flags,
            (untyped)fd,
            (untyped)offset);
    if (ret < 0) 
        return Err(untyped, -ret);
    else
        return Ok(untyped, (untyped)ret);
}


errno_t mmunmap(untyped addr, u64 len) {
    return (errno_t) -(i64)syscall2(SYS_MUNMAP,
            addr,
            (untyped)len);


}
