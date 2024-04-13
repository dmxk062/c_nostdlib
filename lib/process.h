#pragma once

#include "../include/types.h"
#include "syscall.h"


void exit(i64 exitcode);

u64 getpid();

i64 fork();
i64 vfork();

i64 kill(i64 pid, i64 signal);

i64 execve(const char* program, char* const argv[], char* const envp[]);


#define	SIGHUP		1 
#define	SIGINT		2 
#define	SIGQUIT		3 
#define	SIGILL		4 
#define	SIGTRAP		5 
#define	SIGABRT		6 
#define	SIGFPE		8 
#define	SIGKILL		9 
#define	SIGSEGV		11
#define	SIGPIPE		13
#define	SIGALRM		14
#define	SIGTERM		15
