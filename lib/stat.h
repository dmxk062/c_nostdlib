#include "../include/types.h"
#include "syscall.h"
#include "time.h"

/*
 * see https://github.com/torvalds/linux/blob/master/arch/x86/include/uapi/asm/stat.h
 */

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

#define S_IFMT              0170000

#define S_IFDIR             0040000
#define S_IFCHR             0020000
#define S_IFBLK             0060000
#define S_IFREG             0100000
#define S_IFIFO             0010000
#define S_IFLNK             0120000
#define S_IFSOCK            0140000

#define S_SUID              04000
#define S_SGID              02000
#define S_STCK              01000
#define S_READ              0400
#define S_WRITE             0200
#define S_EXEC              0100

// TODO: the rest of the filetype macros
#define S_ISREG(mode) (((mode) & S_IFMT) == S_IFREG)
#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
