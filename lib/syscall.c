#include "types.h"
#include <syscall.h>

untyped syscall6(enum Syscalls call, untyped arg1, untyped arg2, untyped arg3, untyped arg4, untyped arg5, untyped arg6) {
    untyped res;
    asm volatile(
            "movq %1, %%rax;"
            "movq %2, %%rdi;"
            "movq %3, %%rsi;"
            "movq %4, %%rdx;"
            "movq %5, %%r10;"
            "movq %6, %%r8;"
            "movq %7, %%r9;"
            "syscall;"
            "movq %%rax, %0;"
            : "=r" (res)
            : "r" ((u64)call), "r" (arg1), "r"(arg2), "r"(arg3),"r"(arg4), "r"(arg5), "r"(arg6)
            : "rax", "rdi", "rsi", "rdx", "r10", "r8", "r9", "memory"
    );

    return res;
}
