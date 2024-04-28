#pragma once

#include <types.h>


void exit(i64 exitcode);

u64 getpid();

RESULT(u64) fork();
RESULT(u64) vfork();

errno_t kill(i64 pid, i64 signal);

errno_t execve(const char* program, char* const argv[], char* const envp[]);


u64 getuid();
u64 getgid();
u64 geteuid();
u64 getegid();

errno_t setuid(u64 uid);
errno_t setgid(u64 gid);
