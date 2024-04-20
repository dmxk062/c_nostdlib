#include "include/types.h"
#include "lib/string.h"
#include "lib/parsers.h"
#include "lib/format.h"
#include "lib/io.h"
#include "alloc/alloc.h"

i32 main(i32 argc, zstr argv[]) {

    if (argc != 2) {
        return 1;
    }
    result integer = str_to_int(argv[1], strlen(argv[1]), 10);

    if (!integer.success) {
        return 2;
    }

    zstr buffer = malloc(4096);
    if (!buffer) {
        return 3;
    }

    i64 ret = fmt("0x%x\n", buffer, 4096, (fmts){{ .i = (i64)integer.value }});
    if (ret < 0) {
        return 4;
    }
    
    print(buffer);
    free(buffer);

    return 0;
}

