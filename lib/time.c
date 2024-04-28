#include <time.h>
#include <syscall.h>

/* 
 * if sleep was interrupted, the time remaining will be put into remaining
 */
i64 sleep(u64 secs, u64 nanosecs, struct Timespec* remaining) {
    struct Timespec spec;
    spec.secs = secs;
    spec.nano = nanosecs;

    return (i64) syscall2(SYS_NANOSLEEP, &spec, remaining);

}

