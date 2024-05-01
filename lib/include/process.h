#pragma once
#ifndef _PROCESS_H
#define _PROCESS_H

#include "signal.h"
#include <types.h>


void exit(u8 exitcode);

u64 getpid();

RESULT(u64) fork();
RESULT(u64) vfork();

errno_t send_signal(u64 pid, enum Signal sig);

errno_t execve(const char* program, char* const argv[], char* const envp[]);


u64 getuid();
u64 getgid();
u64 geteuid();
u64 getegid();

errno_t setuid(u64 uid);
errno_t setgid(u64 gid);
#endif
