#pragma once
#ifndef _TIME_H
#define _TIME_H

#include <types.h>


struct Timespec {
    u64 secs;
    u64 nano;
};

i64 sleep(u64 secs, u64 nanosecs, struct Timespec* remaining);
#endif
