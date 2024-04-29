#pragma once
#include <types.h>
#include <format.h>

#define FMT_OUTPUT_SIZE 4096



#define STDIN  0
#define STDOUT 1
#define STDERR 2

RESULT(u64) read(u64 fd, void* buffer, u64 count);
RESULT(u64) write(u64 fd, void const* data, u64 count);

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

#define SEEK_SET   0
#define SEEK_CUR   1
#define SEEK_END   2
#define SEEK_DATA  3
#define SEEK_HOLE  4


    
RESULT(u64) open(const char* path, i64 flags, i64 mode);
RESULT(u64) close(u64 fd);
RESULT(u64) seek(u64 fd, i64 offset, u64 origin);

void print(const char* string);

i64 fwrite(u64 fd, const char* format, fmts values);
i64 fprint(const char* format, fmts values);

