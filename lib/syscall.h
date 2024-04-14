#pragma once
#include "../include/types.h"

// TODO: add more syscall numbers
#define SYS_READ        0
#define SYS_WRITE       1

#define SYS_OPEN        2
#define SYS_CLOSE       3

#define SYS_STAT        4
#define SYS_FSTAT       5
#define SYS_LSTAT       6
#define SYS_POLL        7
#define SYS_SEEK        8

#define SYS_MMAP        9

#define SYS_SIGACTION   13
#define SYS_SIGPROCMASC 14
#define SYS_SIGRETURN   15

#define SYS_IOCTL       16

#define SYS_NANOSLEEP   35

#define SYS_GETPID      39

#define SYS_FORK        57
#define SYS_VFORK       58

#define SYS_EXECVE      59
#define SYS_EXIT        60
#define SYS_KILL        62

#define SYS_GETUID      102
#define SYS_GETGID      104
#define SYS_SETUID      105
#define SYS_SETGID      106

#define SYS_GETEUID     107
#define SYS_GETEGID     108



/*
 * Use the one that fits best for your function
 * using fewer registers saves a couple of movqs
 */
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
