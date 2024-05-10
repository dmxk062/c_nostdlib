#pragma once
#ifndef _TIME_H
#define _TIME_H

#include <types.h>

#define UNIX_EPOCH (i64)


struct Timespec {
    u64 secs;
    u64 nano;
};

enum Timezones {
    Timezone_GMT = 0,
    Timezone_CET = 1*60*60,
    Timezone_EET = 2*60*60,

    

};
enum TimezoneDaylightSaving  {
    DaylightSaving_CET = 2*60*60,

};
struct Timezone {
    enum Timezones zone;
    enum TimezoneDaylightSaving ds;
};

typedef struct {
    i64 years;
    i64 days;
    i64 hours;
    i64 minutes;
    i64 seconds;
} Time;

errno_t sleep(u64 secs, u64 nanosecs, struct Timespec* remaining);

errno_t Time_get(struct Timespec* time, struct Timezone* zone);








#endif
