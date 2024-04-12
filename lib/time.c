#include "time.h"

i64 sleep(u64 secs, u64 nanosecs, struct timespec* remaining) {
    struct timespec spec;
    spec.secs = secs;
    spec.nano = nanosecs;

    return (i64)
    syscall2(SYS_NANOSLEEP, &spec, &remaining);

}

