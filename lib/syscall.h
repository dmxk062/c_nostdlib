#pragma once
#include "../include/types.h"

#define SYS_READ   0
#define SYS_WRITE  1

#define SYS_OPEN   2
#define SYS_CLOSE  3

#define SYS_MMAP   9

#define SYS_NANOSLEEP 35

#define SYS_EXIT  60



extern void* syscall0(
        u64 call
);
extern void* syscall1(
        u64 call,
        void* arg1
);
extern void* syscall2(
        u64 call,
        void* arg1,
        void* arg2
);
extern void* syscall3(
        u64 call,
        void* arg1,
        void* arg2,
        void* arg3
);
extern void* syscall4(
        u64 call,
        void* arg1,
        void* arg2,
        void* arg3,
        void* arg4
);
extern void* syscall5(
        u64 call,
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
