#pragma once
#include "types.h"

#define DIRENT_BUF_SIZE 256

enum DT_TYPE {
    DT_UNKNOWN = 0,
    DT_FIFO    = 1,
    DT_CHR     = 2,
    DT_DIR     = 4,
    DT_BLK     = 6,
    DT_REG     = 8,
    DT_LNK     = 10,
    DT_SOCK    = 12,
    DT_WHT     = 14,
};

typedef struct dirent {
    u64     ino;
    u64     off;

    u16     len;
    enum DT_TYPE type : 8;
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

