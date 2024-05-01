#pragma once

#include <types.h>

enum Signal {
    Signal_HUP    = 1,
    Signal_INT    = 2,
    Signal_QUIT   = 3,
    Signal_ILL    = 4,
    Signal_TRAP   = 5,
    Signal_ABRT   = 6,
    Signal_IOT    = 6,
    Signal_BUS    = 7,
    Signal_FPE    = 8,
    Signal_KILL   = 9,
    Signal_USR1   = 10,
    Signal_SEGV   = 11,
    Signal_USR2   = 12,
    Signal_PIPE   = 13,
    Signal_ALRM   = 14,
    Signal_TERM   = 15,
    Signal_STKFLT = 16,
    Signal_CHLD   = 17,
    Signal_CONT   = 18,
    Signal_STOP   = 19,
    Signal_TSTP   = 20,
    Signal_TTIN   = 21,
    Signal_TTOU   = 22,
    Signal_URG    = 23,
    Signal_XCPU   = 24,
    Signal_XFSZ   = 25,
    Signal_VTALRM = 26,
    Signal_PROF   = 27,
    Signal_WINCH  = 28,
    Signal_IO     = 29,
    Signal_POLL   = Signal_IO,
    Signal_PWR    = 30,
    Signal_SYS    = 31,
    Signal_UNUSED = 31,
};

typedef void (*SignalHandler)(i64);
typedef void (*SignalRestorer)(void);

typedef struct {
    u64     bits[2];
} sigset_t;

struct Sigaction {
    SignalHandler handler;
    u64          flags;
    SignalRestorer restorer;
    u64          mask;
};


i64 sigaction(enum Signal signal, struct Sigaction* sa, struct Sigaction* oa);

i64 Signal_action(enum Signal signal, SignalHandler handler, u64 flags, u64 mask, struct Sigaction* old_handler);

extern void sigaction_trampoline();



#define SA_NOCLDSTOP 1
#define SA_NOCLDWAIT 2
#define SA_SIGINFO	 4

#define SA_ONSTACK	 0x08000000
#define SA_RESTART	 0x10000000
#define SA_NODEFER	 0x40000000
#define SA_RESETHAND 0x80000000
#define SA_RESTORER  0x04000000

#define SIG_BLOCK    0
#define SIG_UNBLOCK  1
#define SIG_MASK     2
