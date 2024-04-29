#include <io.h>
#include <syscall.h>
#include <cstring.h>

RESULT(u64) read(u64 fd, void* buffer, u64 count) {
    i64 ret = (i64)syscall3(SYS_READ,
            (void*) (long int) fd,
            (void*) buffer,
            (void*) count);
    if (ret < 0) 
        return (RESULT(u64)){.success = FALSE, .errno = -ret};
    else
        return (RESULT(u64)){.success = TRUE, .value = ret};
    
}

RESULT(u64) write(u64 fd, void const* data, u64 count) {
    i64 ret = (i64)syscall3(SYS_WRITE,
            (void*) (long int) fd,
            (void*) data,
            (void*) count);
    if (ret < 0) 
        return (RESULT(u64)){.success = FALSE, .errno = -ret};
    else
        return (RESULT(u64)){.success = TRUE, .value = ret};
}

RESULT(u64) open(const char *path, i64 flags, i64 mode) {
    i64 ret = (i64)syscall3(SYS_OPEN,
            (void*) path,
            (void*) flags,
            (void*) mode);
    if (ret < 0) 
        return (RESULT(u64)){.success = FALSE, .errno = -ret};
    else
        return (RESULT(u64)){.success = TRUE, .errno = ret};
}

RESULT(u64) close(u64 fd) {
    i64 ret = (i64)syscall1(SYS_CLOSE,
            (void*) fd);
    if (ret < 0) 
        return (RESULT(u64)){.success = FALSE, .errno = -ret};
    else
        return (RESULT(u64)){.success = TRUE, .value = ret};
}


RESULT(u64) seek(u64 fd, i64 offset, u64 origin) {
    i64 ret = (i64)syscall3(SYS_SEEK, 
            (void*)fd,
            (void*)offset,
            (void*)origin);
    if (ret < 0) 
        return (RESULT(u64)){.success = FALSE, .errno = -ret};
    else
        return (RESULT(u64)){.success = TRUE, .value = ret};
}

/*
 * write NULL terminated string to stdout
 */
void print(const char* string) {
    u64 len = strlen(string);
    write(STDOUT, string, len);
}

/*
 * write formatted text to fd
 * maximum size: FMT_OUTPUT_SIZE
 */
i64 fwrite(u64 fd, const char* format, fmts values) {
    char buffer[FMT_OUTPUT_SIZE];

    i64 ret = fmt(format, buffer, FMT_OUTPUT_SIZE, values);
    if (ret > 0 || ret == -3){
        write(fd, buffer, ret);
        return 0;
    }
    return -1;
}

/*
 * print formatted text
 * maximum size: FMT_OUTPUT_SIZE
 */
i64 fprint(const char* format, fmts values) {
    return
    fwrite(STDOUT, format, values);
}
