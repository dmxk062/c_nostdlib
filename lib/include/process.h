#pragma once
#ifndef _PROCESS_H
#define _PROCESS_H

#include "signal.h"
#include <types.h>


void exit(u8 exitcode);

u64 getpid();

RESULT(u64) Process_fork();
RESULT(u64) Process_vfork();

errno_t Process_send_signal(u64 pid, enum Signal sig);

errno_t Process_exec(const zstr program, const zstr argv[], const zstr envv[]);


enum PrctlOp {
    PrctlOp_SET_NAME = 15,
    PrctlOp_GET_NAME = 16,
};
untyped prctl(enum PrctlOp op, untyped arg1, untyped arg2, untyped arg3);
void Process_set_name(char name[16]);
void Process_get_name(char buffer[16]);


u64 Process_get_uid();
u64 Process_get_gid();
u64 Process_get_euid();
u64 Process_get_egid();

errno_t Process_pause();

errno_t Process_set_uid(u64 uid);
errno_t Process_set_gid(u64 gid);


#endif
