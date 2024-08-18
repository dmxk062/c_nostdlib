/*
 * This file is used to initialize various C-library things before main() and to call main()
 * right now it:
 *     - initializes and parses the vdso
 */
#include "alloc.h"
#include "linux/auxv.h"
#include "private/vdso.h"
#include "types.h"
#include <private/libc_start.h>
#include <private/environ.h>

/*
 * Simply crash if we cannot initialize the libc
 */
#define SEGFAULT *((volatile u8*)0)
// #define SEGFAULT


struct __libc_state g_nolibc_global_STATE = {0};
static struct Environment global_env = {0};

i32 __no_libc_start(i32 argc, zstr argv[]) {
    // calculate the position of the environment
    zstr* envv = argv + argc + 1;

    // replace the regular one with hours(initialize malloc in the process)
    errno_t err = Environment_init(&global_env, envv);
    if (err) {
        SEGFAULT;
    }


    g_nolibc_global_STATE.argc = argc;
    g_nolibc_global_STATE.argv = argv;
    g_nolibc_global_STATE.envv = envv;
    g_nolibc_global_STATE.environ = &global_env;

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
    
    i32 return_code = main(argc, argv, envv);
    return return_code;
}
