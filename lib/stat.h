#include "../include/types.h"
#include "syscall.h"

struct stat {
    u64     st_dev;
    u64     st_ino;
    u64     st_nlink;

    u32     st_mode;
    u32     st_uid;
    u32     st_gid;
    u32     __padd0;

    u64     st_rdev;
    i64     st_size;
    i64     st_blksize;
    i64     st_blocks;

    u64     st_atime;
    u64     st_atime_nsec;

    u64     st_mtime;
    u64     st_mtime_nsec;

    u64     st_ctime;
    u64     st_ctime_nsec;

    i64     __unused[3];
};

i64 stat(char* path, struct stat* statbuf);
i64 fstat(u64 fd, struct stat* statbuf);
