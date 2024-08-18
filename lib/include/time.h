#pragma once
#ifndef _TIME_H
#define _TIME_H

#include <types.h>

#define SECONDS_PER_DAY (60 * 60 * 24)

struct Timespec {
    u64 secs;
    u64 nano;
};

enum Timezones {
    Timezone_GMT = 0,
    Timezone_CET = 1 * 60 * 60,
    Timezone_EET = 2 * 60 * 60,

};
enum TimezoneDaylightSaving {
    DaylightSaving_CET = 2 * 60 * 60,

};
struct Timezone {
    enum Timezones zone;
    enum TimezoneDaylightSaving ds;
};

enum Clocks {
    Clock_REALTIME = 0,
    Clock_MONOTONIC = 1,
    Clock_PROCESS_CPUTIME_ID = 1,
    Clock_THREAD_CPUTIME_ID = 1,
    Clock_MONOTONIC_RAW = 4,
    Clock_REALTIME_COARSE = 5,
    Clock_MONOTONIC_COARSE = 6,
    Clock_BOOTTIME = 7,
};

typedef struct {
    u64 years;
    i64 months;
    i64 days;
    i64 day_in_month;
    i64 hours;
    i64 minutes;
    i64 seconds;
} Time;

#define LEAPYEAR(year)                                                         \
    (((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0)) ? true      \
                                                                   : false)
#define DAYS_IN_YEAR(year) (LEAPYEAR(year) ? 366 : 365)

static const i16 DAYS_IN_MONTH[2][12] = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

errno_t sleep(u64 secs, u64 nanosecs, struct Timespec* remaining);

errno_t Time_gettimeofday(struct Timespec* time, struct Timezone* zone);
errno_t Time_gettime(enum Clocks clock, struct Timespec* time);

errno_t Time_locatime(Time* time, struct Timespec* timestamp);

#endif
