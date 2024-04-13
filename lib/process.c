#include "process.h"
#include "syscall.h"


void exit(i64 exitcode) {
    syscall1(SYS_EXIT,
            (void*) exitcode);
}

u64 getpid() {
    return (u64)
    syscall0(SYS_GETPID);
}

i64 fork(){
    return (i64) 
    syscall0(SYS_FORK);
}
i64 vfork(){
    return (i64) 
    syscall0(SYS_VFORK);
}

i64 kill(i64 pid, i64 signal) {
    return (i64)
    syscall2(SYS_KILL,
            (void*)pid,
            (void*)signal);
}

i64 execve(const char *program, char *const *argv, char *const *envp) {
    return (i64)
    syscall3(SYS_EXECVE,
            (void*)program,
            (void*)argv,
            (void*)envp);
}
