#pragma once
#ifndef _IO_H
#define _IO_H

#include <types.h>
#include <format.h>

#define FMT_OUTPUT_SIZE 4096



enum StandardStreams {
    STDIN  = 0,
    STDOUT = 1,
    STDERR = 2,
};

Result(u64) read(u64 fd, void* buffer, u64 count);
Result(u64) write(u64 fd, void const* data, u64 count);

#define O_RDONLY    00000000
#define O_WRONLY    00000001
#define O_RDWR      00000002
#define O_CREAT     00000100
#define O_EXCL      00000200
#define O_APPEND    00002000
#define O_NONBLOCK  00004000
#define O_LARGEFILE 00100000
#define O_DIRECTORY 00200000
#define O_NOFOLLOW  00400000
#define O_NOATIME   01000000
#define O_CLOEXEC   02000000

enum SeekFlag {
    SeekFlag_SET  = 0,
    SeekFlag_CUR  = 1,
    SeekFlag_END  = 2,
    SeekFlag_DATA = 3,
    SeekFlag_HOLE = 4,
};


    
Result(u64) open(const char* path, i64 flags, i64 mode);
Result(u64) close(u64 fd);
Result(u64) seek(u64 fd, i64 offset, enum SeekFlag origin);

void print(const char* string);

Result(u64) fwrite(u64 fd, const zstr format, fmts values);
Result(u64) fprint(const zstr format, fmts values);

#endif
