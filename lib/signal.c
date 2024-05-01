#include "types.h"
#include <signal.h>
#include <syscall.h>

errno_t sigaction(enum Signal signal, struct Sigaction* sa, struct Sigaction* oa) {
     return (errno_t) -(i64)syscall4(SYS_SIGACTION,
            (void*)signal,
            sa,
            oa,
            (void*)8);
}




errno_t Signal_action(enum Signal signal, SignalHandler handler, u64 flags, u64 mask, struct Sigaction* old_handler) {
    struct Sigaction action;
    action.mask = mask;
    action.flags = flags | SA_RESTORER;

    action.handler = handler;
    action.restorer = sigaction_trampoline;

    return
    sigaction(signal, &action, old_handler);

}

