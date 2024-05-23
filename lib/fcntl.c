#include "syscall.h"
#include "types.h"
#include <fcntl.h>

Result(u64) fcntl(u64 fd, enum FcntlOp op, u64 arg) {
    i64 ret = (i64)syscall3(SYS_FCNTL, 
            (untyped)fd, 
            (untyped)op, 
            (untyped)arg);
    if (ret < 0) 
        return Err(u64, -ret);
    else
        return Ok(u64, ret);
}
