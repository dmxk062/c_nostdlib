#pragma once

#include "types.h"


void exit(i64 exitcode);

u64 getpid();

i64 fork();
i64 vfork();

i64 kill(i64 pid, i64 signal);

i64 execve(const char* program, char* const argv[], char* const envp[]);


i64 getuid();
i64 getgid();
i64 geteuid();
i64 getegid();

i64 setuid(i64 uid);
i64 setgid(i64 gid);
