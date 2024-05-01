#include "signal.h"
#include <process.h>
#include <syscall.h>
#include <types.h>


void exit(u8 exitcode) {
    syscall1(SYS_EXIT,
            (untyped) exitcode);
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

errno_t send_signal(u64 pid, enum Signal sig) {
    return (errno_t) -(i64)
    syscall2(SYS_KILL,
            (untyped)pid,
            (untyped)sig);
}

errno_t execve(const char *program, char *const *argv, char *const *envp) {
    return (errno_t) -(i64)
    syscall3(SYS_EXECVE,
            (untyped)program,
            (untyped)argv,
            (untyped)envp);
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
            (untyped)uid);
}
errno_t setgid(u64 gid) {
    return (errno_t)-(i64)
    syscall1(SYS_SETGID,
            (untyped)gid);
}

