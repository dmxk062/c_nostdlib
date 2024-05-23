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

Result(u64) Process_fork(){
    i64 ret = (i64)syscall0(SYS_FORK);
    if (ret < 0) 
        return Err(u64, -ret);
    else
        return Ok(u64, ret);
}
Result(u64) Process_vfork(){
    i64 ret = (i64)syscall0(SYS_VFORK);
    if (ret < 0) 
        return Err(u64, -ret);
    else
        return Ok(u64, ret);
}

errno_t Process_send_signal(u64 pid, enum Signal sig) {
    return (errno_t) -(i64)
    syscall2(SYS_KILL,
            (untyped)pid,
            (untyped)sig);
}

errno_t Process_exec(const zstr program, const zstr argv[], const zstr envv[]) {
    return (errno_t) -(i64)
    syscall3(SYS_EXECVE,
            (untyped)program,
            (untyped)argv,
            (untyped)envv);
}

untyped prctl(enum PrctlOp op, untyped arg1, untyped arg2, untyped arg3) {
    return syscall4(SYS_PRCTL,
            (untyped)op, 
            (untyped)arg1,
            (untyped)arg2,
            (untyped)arg3);
}

void Process_set_name(char name[16]) {
    prctl(PrctlOp_SET_NAME, name, NULL, NULL);
}
void Process_get_name(char buffer[16]) {
    prctl(PrctlOp_GET_NAME, buffer, NULL, NULL);
}

u64 Process_get_uid() {
    return (u64) 
    syscall0(SYS_GETUID);
}
u64 Process_get_gid() {
    return (u64) 
    syscall0(SYS_GETGID);
}
u64 Process_get_euid() {
    return (u64) 
    syscall0(SYS_GETEUID);
}
u64 Process_get_egid() {
    return (u64) 
    syscall0(SYS_GETEGID);
}

errno_t Process_pause() {
    return (errno_t) -(i64)syscall0(SYS_PAUSE);
}

errno_t Process_set_uid(u64 uid) {
    return (errno_t)-(i64)
    syscall1(SYS_SETUID,
            (untyped)uid);
}
errno_t Process_set_gid(u64 gid) {
    return (errno_t)-(i64)
    syscall1(SYS_SETGID,
            (untyped)gid);
}
