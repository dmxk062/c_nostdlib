#include "include/signal.h"
#include "include/syscall.h"

i64 sigaction(u64 signal, struct sigaction* sa, struct sigaction* oa) {
    return (i64) 
    syscall4(SYS_SIGACTION,
            (void*)signal,
            sa,
            oa,
            (void*)8);
}

i64 sigprocmask(i64 how, sigset_t* newset, sigset_t* oldset) {
    return (i64)
    syscall4(SYS_SIGPROCMASC,
            (void*)how,
            newset,
            oldset,
            (void*)8);
}

void sigset_add(sigset_t* set, u64 signum) {
    i64 index = (signum - 1) / (sizeof(u64) * 8);
    i64 offset = (signum - 1) % (sizeof(u64) * 8);

    
    set->bits[index] |= (1UL << offset);
}


i64 setsigaction(u64 signal, sighandler_t handler, u64 flags, u64 mask, struct sigaction* old_handler) {
    struct sigaction action;
    action.mask = mask;
    action.flags = flags | SA_RESTORER;

    action.handler = handler;
    action.restorer = sigaction_trampoline;

    return
    sigaction(signal, &action, old_handler);

}

