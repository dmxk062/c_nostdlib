#include "../include/types.h"
#include "../lib/string.h"
#include "../lib/io.h"
#include "../lib/parsers.h"
#include "../lib/math.h"
#include "../lib/format.h"

i32 main(i32 argc, zstr argv[]) {
    
    if (argc != 3) {
        fprint("%s: float int\n", (fmts){{argv[0]}});
        return 1;
    }
    RESULT(f128) value = str_to_float(argv[1], strlen(argv[1]));
    if (!value.success) {
        fprint("Not a valid floating point number: %s\n", (fmts){
                {.s = argv[1]}
                });
        return 1;
    }
    RESULT(i64) integer = str_to_int(argv[2], strlen(argv[2]), 10);
    if (!integer.success) {
        fprint("Not a valid floating integer: %s\n", (fmts){
                {.s = argv[2]}
                });
        return 1;
    }

    f128 result = root(value.value, integer.value);
    fprint("%F\n", (fmts){
            {.F = {result, .padd=0, .frac=4}}
            });
    return 0;
}
