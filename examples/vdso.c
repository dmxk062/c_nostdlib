#include "format.h"
#include <io.h>
#include <types.h>
#include <linux/auxv.h>
#include <linux/vdso.h>

#define SYMBOL "__vdso_getcpu"

i32 main(i32 argc, zstr argv[], zstr envv[]) {
    Elf64Auxval* auxv = Auxval_find_auxv(envv);

    address vdsoaddr = Auxval_get_value(auxv, AuxvalType_VDSO_ADDRESS)->value;

    i32 (*getcpu)(u32* cpu, u32* node) = VDSO_find_symbol(vdsoaddr, SYMBOL);
    if (!getcpu) {
        print("Failed to get "SYMBOL" from vDSO, aborting\n");
        return 1;
    }

    u32 cpu = 0, node = 0;
    getcpu(&cpu, &node);
    fprint("Running on cpu: %d; NUMA node: %d\n", Fmt({.i = cpu}, {.i = node}));
    return 0;
}
