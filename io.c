#include "io.h"
#include "string.h"
#include "syscall.h"

i64 read(int fd, void* buffer, u64 count) {
    return (i64)
    syscall_raw((void*) SYS_READ,
            (void*) (long int) fd,
            (void*) buffer,
            (void*) count,
            0, 0);
}

i64 write(int fd, void const* data, u64 count) {
    return (i64)
    syscall_raw((void*) SYS_WRITE,
            (void*) (long int) fd,
            (void*) data,
            (void*) count,
            0, 0);
}

i64 open(const char *path, i64 flags, i64 mode) {
    return (i64) 
    syscall_raw((void*) SYS_OPEN,
            (void*) path,
            (void*) flags,
            (void*) mode,
            0, 0);
}

i64 close(u64 fd) {
    return (i64) 
    syscall_raw((void*) SYS_CLOSE,
            (void*) fd,
            0, 0, 0, 0);
}

void puts(const char* string) {
    u64 len = strlen(string);
    write(STDOUT, string, len);
}
