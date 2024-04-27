#pragma once
#include "types.h"

struct dirent {
    u64     inode;
    i64     offset;

    u16     reclen;
    u8      type;
    char    name[256];
};

u64 opendir(const char* path);
