#include "syscall.h"

void exit(i64 exitcode) {
    syscall1(SYS_EXIT,
            (void*) exitcode);
}


i64 ioctl(u64 fd, u64 cmd, u64 arg) {
    return (i64)
    syscall3(SYS_IOCTL,
            (void*)fd,
            (void*)cmd,
            (void*)arg);
}
