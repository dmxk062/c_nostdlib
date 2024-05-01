#pragma once
#ifndef _STAT_H
#define _STAT_H
#include <types.h>
#include <time.h>

/*
 * see https://github.com/torvalds/linux/blob/master/arch/x86/include/uapi/asm/stat.h
 */



/*
 * Extremely little-Endian, amd64 and linux specific
 * it could be considered a crime,
 * but it's a lot nicer to use
 */
union StatMode {
    u16 st_mode_bits;
    union {
        u16 perms : 12;
        struct {
            /*
             * From right to left:
             * e.g. 000 111 101 101 : 755
             */
            bool    oexec  : 1;
            bool    owrite : 1;
            bool    oread  : 1;

            bool    gexec  : 1;
            bool    gwrite : 1;
            bool    gread  : 1;

            bool    uexec  : 1;
            bool    uwrite : 1;
            bool    uread  : 1;

            bool    sticky : 1;
            bool    setgid : 1;
            bool    setuid : 1;

            u8      type : 4;
        };
    };
};

struct Stat {
    u64     st_dev;
    u64     st_ino;
    u64     st_nlink;

    union StatMode mode;
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
    struct Timespec atime;
    struct Timespec mtime;
    struct Timespec ctime;
    i64     __unused[3];
};


errno_t stat(char* path, struct Stat* statbuf);
errno_t lstat(char* path, struct Stat* statbuf);
errno_t fstat(u64 fd, struct Stat* statbuf);

#define	S_DIR     004
#define	S_CHR     002
#define	S_BLK     006
#define	S_REG     010
#define	S_IFO     001
#define	S_LNK     012
#define	S_SOCK    014

#endif
