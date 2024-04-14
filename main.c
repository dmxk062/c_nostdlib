#include "lib/io.h"
#include "lib/time.h"
#include "lib/syscall.h"
#include "lib/process.h"
#include "lib/string.h"
#include "lib/signal.h"

void* crash = NULL;

bool had_sig = FALSE;



void signal_handler(i64 signum) {
    return;
}
void signal_restorer(void) {
    asm("mov $15, %rax\nsyscall");
}
int main(int argc, char* argv[]) {
    struct sigaction sa;
    struct sigaction oa;
    sa.handler = signal_handler;
    sa.flags = SA_RESTORER;
    sa.mask = 0;
    sa.restorer = signal_restorer;


    i64 ret = 0;
    ret = sigaction(SIGINT, &sa, &oa);
    syscall0(0x22); // pause


    return 0;
}
