#include "types.h"
#include <io.h>
#include <syscall.h>
#include <cstring.h>

Result(u64) read(u64 fd, void* buffer, u64 count) {
    i64 ret = (i64)syscall3(SYS_READ,
            (void*) (long int) fd,
            (void*) buffer,
            (void*) count);
    if (ret < 0) 
        return Err(u64, -ret);
    else
        return Ok(u64, ret);
    
}

Result(u64) write(u64 fd, void const* data, u64 count) {
    i64 ret = (i64)syscall3(SYS_WRITE,
            (void*) (long int) fd,
            (void*) data,
            (void*) count);
    if (ret < 0) 
        return Err(u64, -ret);
    else
        return Ok(u64, ret);
}

Result(u64) open(const char *path, i64 flags, i64 mode) {
    i64 ret = (i64)syscall3(SYS_OPEN,
            (void*) path,
            (void*) flags,
            (void*) mode);
    if (ret < 0) 
        return Err(u64, -ret);
    else
        return Ok(u64, ret);
}

Result(u64) close(u64 fd) {
    i64 ret = (i64)syscall1(SYS_CLOSE,
            (void*) fd);
    if (ret < 0) 
        return Err(u64, -ret);
    else
        return Ok(u64, ret);
}


Result(u64) seek(u64 fd, i64 offset, enum SeekFlag origin) {
    i64 ret = (i64)syscall3(SYS_SEEK, 
            (void*)fd,
            (void*)offset,
            (void*)origin);
    if (ret < 0) 
        return Err(u64, -ret);
    else
        return Ok(u64, ret);
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
Result(u64) fwrite(u64 fd, const char* format, fmts values) {
    char buffer[FMT_OUTPUT_SIZE];

    Result(u64) ret = fmt(format, buffer, FMT_OUTPUT_SIZE, values);
    if (!ret.ok) {
        return ret;
    }
    write(fd, buffer, ret.value);
    return ret;
}

/*
 * print formatted text
 * maximum size: FMT_OUTPUT_SIZE
 */
Result(u64) fprint(const char* format, fmts values) {
    return
    fwrite(STDOUT, format, values);
}
