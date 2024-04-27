#include "include/ioctl.h"
#include "include/syscall.h"

RESULT(u64) ioctl(u64 fd, u64 cmd, void* arg) {
    i64 ret = (i64)syscall3(SYS_IOCTL,
            (void*)fd,
            (void*)cmd,
            arg);
    if (ret < 0) 
        return (RESULT(u64)){.success = FALSE, .errno = -ret};
    else
        return (RESULT(u64)){.success = TRUE, .value = ret};
}

