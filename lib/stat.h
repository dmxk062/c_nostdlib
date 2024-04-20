#include "../include/types.h"
#include "syscall.h"
#include "time.h"

/*
 * see https://github.com/torvalds/linux/blob/master/arch/x86/include/uapi/asm/stat.h
 */



union struct_mode {
    u32 st_mode_bits;
    union {
        union {
            u16 perms : 12;
            struct {

                bool    oread  : 1;
                bool    owrite : 1;
                bool    oexec  : 1;

                bool    gread  : 1;
                bool    gwrite : 1;
                bool    gexec  : 1;

                bool    uread  : 1;
                bool    uwrite : 1;
                bool    uexec  : 1;

                bool    suid   : 1;
                bool    sgid   : 1;
                bool    sticky : 1;

            };
        };
        struct {
            u32 : 12;
            u8  type : 4;
        };
    };

};

struct stat {
    u64     st_dev;
    u64     st_ino;
    u64     st_nlink;

    union struct_mode mode;
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

#define	S_DIR     004
#define	S_CHR     002
#define	S_BLK     006
#define	S_REG     010
#define	S_IFO     001
#define	S_LNK     012
#define	S_SOCK    014
