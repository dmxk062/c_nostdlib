#pragma once

#ifndef _DIRENT_H
#define _DIRENT_H


#include <types.h>

#define DIRENT_BUF_SIZE 256

enum DirentType {
    DirentType_UNKNOWN = 0,
    DirentType_FIFO    = 1,
    DirentType_CHR     = 2,
    DirentType_DIR     = 4,
    DirentType_BLK     = 6,
    DirentType_REG     = 8,
    DirentType_LNK     = 10,
    DirentType_SOCK    = 12,
    DirentType_WHT     = 14,
};

typedef struct dirent {
    u64     ino;
    u64     off;

    u16     len;
    enum DirentType type : 8;
    char    name[5];
} Dirent;

typedef struct directory_buffer {
    u64     offset;
    u64     len;
    char    buffer[DIRENT_BUF_SIZE];
} DirectoryBuffer;

Result(u64) opendir(const char* path);

Result(u64) _getdents(u64 fd, Dirent* ent, u64 count);

DefineResult(Dirent*, Dirent);
Result(Dirent) nextdir(u64 fd, DirectoryBuffer* buf);

#endif
