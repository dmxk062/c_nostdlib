#pragma once

#include "../include/types.h"
#include "syscall.h"


struct timespec {
    u64 secs;
    u64 nano;
};

i64 sleep(u64 secs, u64 nanosecs, struct timespec* remaining);
