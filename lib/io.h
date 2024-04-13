#pragma once
#include "../include/types.h"
#include "string.h"
#include "syscall.h"


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

    
i64 open(const char* path, i64 flags, i64 mode);
i64 close(u64 fd);

void puts(const char* string);
