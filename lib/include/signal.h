#pragma once

#include <types.h>

typedef void (*sighandler_t)(i64);
typedef void (*sigrestore_t)(void);

typedef struct {
    u64     bits[2];
} sigset_t;

struct Sigaction {
    sighandler_t handler;
    u64          flags;
    sigrestore_t restorer;
    u64          mask;
};


i64 sigaction(u64 signal, struct Sigaction* sa, struct Sigaction* oa);
i64 sigprocmask(i64 how, sigset_t* newset, sigset_t* oldset);

i64 setsigaction(u64 signal, sighandler_t handler, u64 flags, u64 mask, struct Sigaction* old_handler);

void sigset_add(sigset_t* set, u64 signum);
extern void sigaction_trampoline();


#define SIGHUP		 1
#define SIGINT		 2
#define SIGQUIT		 3
#define SIGILL		 4
#define SIGTRAP		 5
#define SIGABRT		 6
#define SIGIOT		 6
#define SIGBUS		 7
#define SIGFPE		 8
#define SIGKILL		 9
#define SIGUSR1		10
#define SIGSEGV		11
#define SIGUSR2		12
#define SIGPIPE		13
#define SIGALRM		14
#define SIGTERM		15
#define SIGSTKFLT	16
#define SIGCHLD		17
#define SIGCONT		18
#define SIGSTOP		19
#define SIGTSTP		20
#define SIGTTIN		21
#define SIGTTOU		22
#define SIGURG		23
#define SIGXCPU		24
#define SIGXFSZ		25
#define SIGVTALRM	26
#define SIGPROF		27
#define SIGWINCH	28
#define SIGIO		29
#define SIGPOLL		SIGIO
#define SIGPWR		30
#define SIGSYS		31
#define	SIGUNUSED	31

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
