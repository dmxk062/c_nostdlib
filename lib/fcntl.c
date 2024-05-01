#include "syscall.h"
#include "types.h"
#include <fcntl.h>

RESULT(u64) fcntl(u64 fd, enum FcntlOp op, u64 arg) {
    i64 ret = (i64)syscall3(SYS_FCNTL, 
            (untyped)fd, 
            (untyped)op, 
            (untyped)arg);
    if (ret < 0) 
        return (RESULT(u64)){.success = FALSE, .errno = -ret};
    else
        return (RESULT(u64)){.success = TRUE, .value = ret};
}
