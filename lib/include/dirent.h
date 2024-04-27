#pragma once
#include "types.h"

#define DIRENT_BUF_SIZE 256

typedef struct dirent {
    u64     ino;
    u64     off;

    u16     len;
    u8      type;
    char    name[5];
} dirent;

typedef struct directory_buffer {
    u64     offset;
    u64     len;
    char    buffer[DIRENT_BUF_SIZE];
} directory_buffer;

RESULT(u64) opendir(const char* path);

RESULT(u64) _getdents(u64 fd, dirent* ent, u64 count);

DEFRESULT(dirent*, dirent);
RESULT(dirent) nextdir(u64 fd, directory_buffer* buf);

#define DT_UNKNOWN  0
#define DT_FIFO     1
#define DT_CHR      2
#define DT_DIR      4
#define DT_BLK      6
#define DT_REG      8
#define DT_LNK      10
#define DT_SOCK     12
#define DT_WHT      14

