#include "io.h"
#include "time.h"
#include "types.h"
#include <linux/auxv.h>
#include <linux/vdso.h>

typedef int (*gettimeofday_t)(struct Timespec* t, struct Timespec* z);

i32 main(i32 argc, zstr argv[], zstr envv[]) {
    Elf64Auxval* auxv = Auxval_find_auxv(envv);

    address vdsoaddr = (address)Auxval_get_value(auxv, AuxvalType_VDSO_ADDRESS)->value;

    address addr = VDSO_find_symbol(vdsoaddr, "gettimeofday");
    if (!addr) {
        fwrite(STDERR, "Failed to find symbol", NULL);
        return 1;
    };

    gettimeofday_t gettimeofday = (gettimeofday_t)addr;
    struct Timespec sp;
    gettimeofday(&sp, NULL);
    fprint("Seconds from VDSO gettimeofday: %d\n", (fmts){{.i = sp.secs}});
    return 0;
}
