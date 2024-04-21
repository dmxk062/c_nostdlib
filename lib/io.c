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


i64 seek(u64 fd, i64 offset, u64 origin) {
    return (i64) 
    syscall3(SYS_SEEK, 
            (void*)fd,
            (void*)offset,
            (void*)origin);
}

/*
 * write NULL terminated string to stdout
 */
void print(const char* string) {
    u64 len = strlen(string);
    write(STDOUT, string, len);
}

i64 fwrite(u64 fd, const char* format, fmts values) {
    char buffer[FMT_OUTPUT_SIZE];

    i64 ret = fmt(format, buffer, FMT_OUTPUT_SIZE, values);
    if (ret > 0 || ret == -3){
        write(fd, buffer, ret);
        return 0;
    }
    return -1;
}

i64 fprint(const char* format, fmts values) {
    return
    fwrite(STDOUT, format, values);
}
