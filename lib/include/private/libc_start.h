#pragma once
#ifndef _PRIVATE_LIBC_START_H
#define _PRIVATE_LIBC_START_H

#include <private/vdso.h>
#include <linux/auxv.h>

struct __libc_state {
    i32 argc;
    zstr* argv;
    zstr* envv;
    Elf64Auxval* auxv;
    struct VDSO vdso;
};

extern struct __libc_state g_nolibc_global_STATE;



extern i32 main(i32, zstr[], zstr[]);
i32 __no_libc_start(i32 argc, zstr argv[], zstr envv[]);

#endif
