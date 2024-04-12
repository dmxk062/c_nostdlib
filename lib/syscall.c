#include "syscall.h"

void exit(i64 exitcode) {
    syscall1(SYS_EXIT,
            (void*) exitcode);
}

i64 sleep(u64 secs, u64 nanosecs) {
    struct timespec spec;
    spec.secs = secs;
    spec.nano = nanosecs;

    return (i64)
    syscall1(SYS_NANOSLEEP, &spec);

}
