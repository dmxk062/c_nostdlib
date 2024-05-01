#include "syscall.h"
#include "types.h"
#include "filesystem.h"

errno_t  unlink(const zstr path) {
    return (errno_t) -(i64)syscall1(SYS_UNLINK,
            path);
}

errno_t access(const zstr path, u64 mode) {
    return (errno_t) -(i64)syscall2(SYS_ACCESS,
            path,
            (untyped)mode);
}
