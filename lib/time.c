#include "types.h"
#include <time.h>
#include <syscall.h>

/* 
 * if sleep was interrupted, the time remaining will be put into remaining
 */
errno_t sleep(u64 secs, u64 nanosecs, struct Timespec* remaining) {
    struct Timespec spec;
    spec.secs = secs;
    spec.nano = nanosecs;

    return (errno_t) -(i64)syscall2(SYS_NANOSLEEP, &spec, remaining);

}

errno_t Time_get(struct Timespec* time, struct Timezone* zone) {
    return (errno_t) -(i64)syscall2(SYS_GETTIMEOFDAY, time, zone);
}

