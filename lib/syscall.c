#include "include/syscall.h"

void exit(i64 exitcode) {
    syscall_raw((void*)SYS_EXIT,
            (void*) exitcode,
            0, 0, 0, 0);
}
