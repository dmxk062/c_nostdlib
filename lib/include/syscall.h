#pragma once
#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <types.h>

// TODO: add more syscall numbers
enum Syscalls {
/*
 * I/O
 */
SYS_READ  = 0,
SYS_WRITE = 1,
SYS_OPEN  = 2,
SYS_CLOSE = 3,

/*
 * Non I/O file operations
 */
SYS_STAT  = 4,
SYS_FSTAT = 5,
SYS_LSTAT = 6,
SYS_POLL  = 7,
SYS_SEEK  = 8,
SYS_MMAP  = 9,

SYS_MUNMAP = 11,

/*
 * Signals
 */
SYS_SIGACTION   = 13,
SYS_SIGPROCMASC = 14,
SYS_SIGRETURN   = 15,

SYS_IOCTL = 16,

SYS_ACCESS = 21,
SYS_PIPE   = 22,

SYS_DUP       = 32,
SYS_DUP2      = 33,

SYS_PAUSE     = 34,
SYS_NANOSLEEP = 35,

SYS_GETPID = 39,

/*
 * Socket calls
 */
SYS_SOCKET   = 41,
SYS_CONNECT  = 42,
SYS_ACCEPT   = 43,
SYS_SENDTO   = 44,
SYS_RECVFROM = 45,
SYS_SENDMSG  = 46,
SYS_RECVMSG  = 47,
SYS_SHUTDOWN = 48,
SYS_BIND     = 49,
SYS_LISTEN   = 50,

SYS_FORK = 57,
SYS_VFORK = 58,

SYS_EXECVE = 59,
SYS_EXIT = 60,
SYS_KILL = 62,

SYS_FCNTL = 72,

SYS_GETCWD = 79,
SYS_CHDIR  = 80,
/*
 * Filesystem
 */

SYS_LINK = 86,
SYS_UNLINK = 87,

SYS_GETTIMEOFDAY = 96,

SYS_GETDENTS = 78,
SYS_GETDENTS64 = 217,

SYS_GETUID = 102,
SYS_GETGID = 104,
SYS_SETUID = 105,
SYS_SETGID = 106,

SYS_GETEUID = 107,
SYS_GETEGID = 108,

SYS_PRCTL = 157,

SYS_CLOCK_GETTIME = 228,

};



/*
 * Use the one that fits best for your function
 * using fewer registers saves a couple of movqs
 */
extern untyped syscall0(
        enum Syscalls call
);
extern untyped syscall1(
        enum Syscalls call,
        untyped arg1
);
extern untyped syscall2(
        enum Syscalls call,
        untyped arg1,
        untyped arg2
);
extern untyped syscall3(
        enum Syscalls call,
        untyped arg1,
        untyped arg2,
        untyped arg3
);
extern untyped syscall4(
        enum Syscalls call,
        untyped arg1,
        untyped arg2,
        untyped arg3,
        untyped arg4
);
extern untyped syscall5(
        enum Syscalls call,
        untyped arg1,
        untyped arg2,
        untyped arg3,
        untyped arg4,
        untyped arg5
);
untyped syscall6(enum Syscalls call, untyped arg1, untyped arg2, untyped arg3, untyped arg4, untyped arg5, untyped arg6);

#endif
