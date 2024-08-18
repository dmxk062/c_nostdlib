#pragma once
#ifndef _PRIVATE_LIBC_START_H
#define _PRIVATE_LIBC_START_H

#include <private/environ.h>
#include <private/vdso.h>
#include <linux/auxv.h>

struct __libc_state {
    /* original argc of the program */
    i32 argc;
    /* original argv */
    zstr* argv;
    /* original environment, not used by us */
    zstr* envv;
    /* higher level, heap based environment for us */
    struct Environment* environ;
    /* the auxiliary vector */
    Elf64Auxval* auxv;
    /* the virtual dynamic shared object and its functions */
    struct VDSO vdso;
};

extern struct __libc_state g_nolibc_global_STATE;



extern i32 main(i32, zstr[], zstr[]);
i32 __no_libc_start(i32 argc, zstr argv[]);

#endif
