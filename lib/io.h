#include "../include/types.h"

#define STDIN  0
#define STDOUT 1
#define STDERR 2

i64 read(int fd, void* buffer, u64 count);
i64 write(int fd, void const* data, u64 count);

#define O_RD 00
#define O_WR 01
#define O_RW 02
    
i64 open(const char* path, i64 flags, i64 mode);
i64 close(u64 fd);

void puts(const char* string);
