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
int main(int argc, char* argv[]) {

    setsigaction(SIGINT, signal_handler, 0, 0, NULL);
    syscall0(0x22);
    return 0;
}
