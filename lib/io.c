#include "io.h"

i64 read(int fd, void* buffer, u64 count) {
    return (i64)
    syscall3(SYS_READ,
            (void*) (long int) fd,
            (void*) buffer,
            (void*) count);
}

i64 write(int fd, void const* data, u64 count) {
    return (i64)
    syscall3(SYS_WRITE,
            (void*) (long int) fd,
            (void*) data,
            (void*) count);
}

i64 open(const char *path, i64 flags, i64 mode) {
    return (i64) 
    syscall3(SYS_OPEN,
            (void*) path,
            (void*) flags,
            (void*) mode);
}

i64 close(u64 fd) {
    return (i64) 
    syscall1(SYS_CLOSE,
            (void*) fd);
}

/*
 * FIXME: naming
 * not standards conformant, doesnt print a newline
 * should probably be renamed to `print` 
 */
void puts(const char* string) {
    u64 len = strlen(string);
    write(STDOUT, string, len);
}
