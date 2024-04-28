#include "include/process.h"
#include "include/syscall.h"
#include "types.h"


void exit(i64 exitcode) {
    syscall1(SYS_EXIT,
            (void*) exitcode);
}

u64 getpid() {
    return (u64)
    syscall0(SYS_GETPID);
}

RESULT(u64) fork(){
    i64 ret = (i64)syscall0(SYS_FORK);
    if (ret < 0) 
        return (RESULT(u64)){.success = FALSE, .errno = -ret};
    else
        return (RESULT(u64)){.success = TRUE, .value = ret};
}
RESULT(u64) vfork(){
    i64 ret = (i64)syscall0(SYS_VFORK);
    if (ret < 0) 
        return (RESULT(u64)){.success = FALSE, .errno = -ret};
    else
        return (RESULT(u64)){.success = TRUE, .value = ret};
}

errno_t kill(i64 pid, i64 signal) {
    return (errno_t)-(i64)
    syscall2(SYS_KILL,
            (void*)pid,
            (void*)signal);
}

errno_t execve(const char *program, char *const *argv, char *const *envp) {
    return (errno_t)-(i64)
    syscall3(SYS_EXECVE,
            (void*)program,
            (void*)argv,
            (void*)envp);
}

u64 getuid() {
    return (u64) 
    syscall0(SYS_GETUID);
}
u64 getgid() {
    return (u64) 
    syscall0(SYS_GETGID);
}
u64 geteuid() {
    return (u64) 
    syscall0(SYS_GETEUID);
}
u64 getegid() {
    return (u64) 
    syscall0(SYS_GETEGID);
}

errno_t setuid(u64 uid) {
    return (errno_t)-(i64)
    syscall1(SYS_SETUID,
            (void*)uid);
}
errno_t setgid(u64 gid) {
    return (errno_t)-(i64)
    syscall1(SYS_SETGID,
            (void*)gid);
}

