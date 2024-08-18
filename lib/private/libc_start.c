/*
 * This file is used to initialize various C-library things before main() and to call main()
 * right now it:
 *     - initializes and parses the vdso
 */
#include "linux/auxv.h"
#include "private/vdso.h"
#include "types.h"
#include <private/libc_start.h>

/*
 * Simply crash if we cannot initialize the libc
 */
#define SEGFAULT *((volatile u8*)0)

struct __libc_state g_nolibc_global_STATE = {0};

i32 __no_libc_start(i32 argc, zstr argv[], zstr envv[]) {
    g_nolibc_global_STATE.argc = argc;
    g_nolibc_global_STATE.argv = argv;
    g_nolibc_global_STATE.envv = envv;

    Elf64Auxval* auxv = Auxval_find_auxv(envv);
    if (!auxv) {
        SEGFAULT;
    }
    g_nolibc_global_STATE.auxv = auxv;

    address vdso_addr = Auxval_get_value(auxv, AuxvalType_VDSO_ADDRESS)->value;
    if (!vdso_addr) {
        SEGFAULT;
    }

    VDSO_init(vdso_addr);
    
    return main(argc, argv, envv);
}
