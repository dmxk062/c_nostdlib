#include <io.h>
#include <types.h>
#include <process.h>
#include <signal.h>

volatile i64 received_sig = 0;

void signal_handler(i64 sig) {
    fprint("Received signal %d\n", (fmts){{.i = sig}});
    received_sig = sig;
    return;
} 
i32 main(i32 argc, zstr argv[]) {

    for (u64 sig = 1; sig < Signal_RT_MAX; sig++) {
        Signal_action(sig, signal_handler, 0, 0, NULL);;
    }

    Process_pause();
    return received_sig;

}
