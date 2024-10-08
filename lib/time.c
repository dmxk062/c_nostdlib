#include "types.h"
#include <time.h>
#include <syscall.h>
#include <private/libc_start.h>

/* 
 * if sleep was interrupted, the time remaining will be put into remaining
 */
errno_t sleep(u64 secs, u64 nanosecs, struct Timespec* remaining) {
    struct Timespec spec = {
        .secs = secs,
        .nano = nanosecs,
    };

    return (errno_t) -(i64)syscall2(SYS_NANOSLEEP, &spec, remaining);

}

errno_t Time_gettimeofday(struct Timespec* time, struct Timezone* zone) {
    if (g_nolibc_global_STATE.vdso.funcs.clock_gettimeofday) {
        return (errno_t) -(g_nolibc_global_STATE.vdso.funcs.clock_gettimeofday(time, zone));
    } else {
        return (errno_t) -(i64)syscall2(SYS_GETTIMEOFDAY, time, zone);
    }
}

errno_t Time_gettime(enum Clocks clock, struct Timespec* time) {
    if (g_nolibc_global_STATE.vdso.funcs.clock_gettimeofday) {
        return (errno_t) -(g_nolibc_global_STATE.vdso.funcs.clock_gettime(clock, time));
    } else {
        return (errno_t) -(i64)syscall2(SYS_CLOCK_GETTIME, (untyped)clock, time);
    }
}

errno_t Time_locatime(Time* time, struct Timespec* timestamp) {

    i64 daytime = timestamp->secs % SECONDS_PER_DAY;
    i64 daynumber = timestamp->secs / SECONDS_PER_DAY;

    time->seconds = daytime % 60;
    time->minutes = (daytime % 3600) / 60;
    time->hours   = daytime / 3600;

    i64 year = 1970; /* UNIX epoch */
    while (daynumber >= DAYS_IN_YEAR(year)){
        daynumber -= DAYS_IN_YEAR(year);
        year++;
    }
    time->years = year;
    time->days  = daynumber;
    
    time->months = 0;
    while (daynumber >= (DAYS_IN_MONTH[LEAPYEAR(year)][time->months])) {
        daynumber -= DAYS_IN_MONTH[LEAPYEAR(year)][time->months];
        time->months++;
    }
    time->day_in_month = daynumber+1;

    return 0;

}

