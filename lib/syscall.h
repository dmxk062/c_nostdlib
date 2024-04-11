#include "../include/types.h"

#define SYS_READ 0
#define SYS_WRITE 1

#define SYS_OPEN 2
#define SYS_CLOSE 3

#define SYS_EXIT 60


void* syscall_raw(
        void* call,
        void* arg1,
        void* arg2,
        void* arg3,
        void* arg4,
        void* arg5
);


void exit(i64 exitcode);
