#include "syscall.h"

void exit(i64 exitcode) {
    syscall1(SYS_EXIT,
            (void*) exitcode);
}

