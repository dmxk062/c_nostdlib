#include <ioctl.h>
#include <syscall.h>

Result(u64) ioctl(u64 fd, u64 cmd, void* arg) {
    i64 ret = (i64)syscall3(SYS_IOCTL,
            (void*)fd,
            (void*)cmd,
            arg);
    if (ret < 0) 
        return Err(u64, -ret);
    else
        return Ok(u64, ret);
}

