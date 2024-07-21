#include "io.h"
#include "time.h"
#include "types.h"
#include <linux/auxv.h>
#include <linux/vdso.h>

#define SYMBOL "__vdso_gettimeofday"

i32 main(i32 argc, zstr argv[], zstr envv[]) {
    Elf64Auxval* auxv = Auxval_find_auxv(envv);

    address vdsoaddr = Auxval_get_value(auxv, AuxvalType_VDSO_ADDRESS)->value;

    i32 (*gettimeofday)(struct Timespec* t, struct Timespec* z) = VDSO_find_symbol(vdsoaddr, SYMBOL);
    if (!gettimeofday) {
        print("Failed to find "SYMBOL" in the vDSO\n");
        return 1;
    }


    struct Timespec sp = {0};
    gettimeofday(&sp, NULL);
    fprint(SYMBOL"()={ secs = %d, nano = %d}\n", (fmts){{.i = sp.secs}, {.i = sp.nano}});
    return 0;
}
