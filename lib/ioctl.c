#include "ioctl.h"
i64 ioctl(u64 fd, u64 cmd, void* arg) {
    return (i64)
    syscall3(SYS_IOCTL,
            (void*)fd,
            (void*)cmd,
            arg);
}

