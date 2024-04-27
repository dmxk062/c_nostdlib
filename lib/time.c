#include "include/time.h"
#include "include/syscall.h"

/* 
 * if sleep was interrupted, the time remaining will be put into remaining
 */
i64 sleep(u64 secs, u64 nanosecs, struct timespec* remaining) {
    struct timespec spec;
    spec.secs = secs;
    spec.nano = nanosecs;

    return (i64) syscall2(SYS_NANOSLEEP, &spec, remaining);

}

