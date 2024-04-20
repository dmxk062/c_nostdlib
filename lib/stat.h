#include "../include/types.h"
#include "syscall.h"
#include "time.h"

/*
 * see https://github.com/torvalds/linux/blob/master/arch/x86/include/uapi/asm/stat.h
 */




struct stat_mode {
    u8 : 3; // padd

    bool    isreg  : 1;
    bool    isdir  : 1;
    bool    ischr  : 1;
    bool    isblk  : 1;
    bool    isfifo : 1;
    bool    issock : 1;


    u8 : 2; // padd
    bool    suid : 1;
    bool    sgid : 1;
    bool    stck : 1;

    bool    uread  : 1;
    bool    uwrite : 1;
    bool    uexec  : 1;

    bool    gread  : 1;
    bool    gwrite : 1;
    bool    gexec  : 1;

    bool    oread  : 1;
    bool    owrite : 1;
    bool    oexec  : 1;

};

struct stat {
    u64     st_dev;
    u64     st_ino;
    u64     st_nlink;

    union {
        u32              st_mode_num;
        struct stat_mode st_mode;
    };
    u32     st_uid;
    u32     st_gid;
    u32     : 4;

    u64     st_rdev;
    i64     st_size;
    i64     st_blksize;
    i64     st_blocks;

    /* 
     * All of these fields are equivalent to:
     * u64 seconds;
     * u64 nanoseconds;
     * the struct makes it nicer to access
     */
    struct timespec atime;
    struct timespec mtime;
    struct timespec ctime;
    i64     __unused[3];
};


i64 stat(char* path, struct stat* statbuf);
i64 lstat(char* path, struct stat* statbuf);
i64 fstat(u64 fd, struct stat* statbuf);
