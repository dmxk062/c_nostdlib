#include "syscall.h"

void exit(i64 exitcode) {
    syscall_raw((void*)SYS_EXIT,
            (void*) exitcode,
            0, 0, 0, 0);
}

i64 sleep(u64 secs, u64 nanosecs) {
    struct timespec spec;
    spec.secs = secs;
    spec.nano = nanosecs;

    return (i64)
    syscall_raw((void*)SYS_NANOSLEEP, &spec, 
        0, 0, 0, 0);

}
