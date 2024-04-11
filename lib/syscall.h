#pragma once
#include "../include/types.h"

#define SYS_READ   0
#define SYS_WRITE  1

#define SYS_OPEN   2
#define SYS_CLOSE  3

#define SYS_MMAP   9

#define SYS_BRK   12

#define SYS_EXIT  60

#define SYS_NANOSLEEP 35


void* syscall_raw(
        void* call,
        void* arg1,
        void* arg2,
        void* arg3,
        void* arg4,
        void* arg5
);


void exit(i64 exitcode);

struct timespec {
    u64 secs;
    u64 nano;
};
i64 sleep(u64 secs, u64 nanosecs);
