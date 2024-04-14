#pragma once

#include "../include/types.h"
#include "syscall.h"


void exit(i64 exitcode);

u64 getpid();

i64 fork();
i64 vfork();

i64 kill(i64 pid, i64 signal);

i64 execve(const char* program, char* const argv[], char* const envp[]);

