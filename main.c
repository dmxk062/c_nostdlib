#include "include/types.h"
#include "lib/string.h"
#include "lib/parsers.h"
#include "lib/format.h"
#include "lib/io.h"
#include "lib/stat.h"
#include "alloc/alloc.h"



i32 main(i32 argc, zstr argv[]) {


    const char input[] = "-15687956854";
    RESULT(i64) number = str_to_int(input, strlen(input), 10);

    if (number.success) {
    fprint("%x\n", (fmts){
            {.i = number.value}
            });
    }


    

    return 0;
}

