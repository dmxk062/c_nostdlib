#pragma once
#include "../include/types.h"
#include "string.h"
#include "syscall.h"
#include "format.h"

#define FMT_OUTPUT_SIZE 4096



#define STDIN  0
#define STDOUT 1
#define STDERR 2

i64 read(int fd, void* buffer, u64 count);
i64 write(int fd, void const* data, u64 count);

#define O_RDONLY 00
#define O_WRONLY 01
#define O_RDWR   02
#define O_EXCL   00200
#define O_CREAT  001000
#define O_APPEND 002000

#define SEEK_SET   0
#define SEEK_CUR   1
#define SEEK_END   2
#define SEEK_DATA  3
#define SEEK_HOLE  4


    
i64 open(const char* path, i64 flags, i64 mode);
i64 close(u64 fd);
i64 seek(u64 fd, i64 offset, u64 origin);

void print(const char* string);

i64 fwrite(u64 fd, const char* format, fmts values);
i64 fprint(const char* format, fmts values);

