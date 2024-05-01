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

errno_t getcwd(char* buf, u64 size) {
    return (errno_t) -(i64)syscall2(SYS_GETCWD,
            buf, (untyped)size);
}

errno_t chdir(const zstr path) {
    return (errno_t) -(i64)syscall1(SYS_CHDIR, path);
}
