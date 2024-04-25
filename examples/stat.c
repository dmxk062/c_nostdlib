#include "include/types.h"
#include "lib/string.h"
#include "lib/io.h"
#include "lib/parsers.h"
#include "lib/format.h"

i32 main(i32 argc, zstr argv[]) {
    i64 base = 16;
    RESULT(i64) number;
    if (argc > 1) {
        number = str_to_int(argv[1], strlen(argv[1]), 10);
        if (number.success) 
            base = number.value;
    }

    char buffer[1024];
    i64 num_read;

    while (TRUE) {
        num_read = read(STDIN, buffer, 1024);
        if (num_read <= 0) {
            break;
            }
            number = str_to_int(buffer, num_read - 1, base);

        if (number.success) {
            fprint("%b\n", (fmts){
                        {.i = number.value}
                    });
            }
    }

    return 0;
}
