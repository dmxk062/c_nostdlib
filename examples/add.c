#include <types.h>
#include <io.h>
#include <args.h>

#define VERSION "beta-1.0"

i32 main(i32 argc, zstr argv[]) {

    bool show_version = FALSE;

    f128 number1 = 0;
    i64  num1index = 0;
    bool  num1success = FALSE;
    f128 number2 = 0;
    i64  num2index = 0;
    bool  num2success = FALSE;

    named_args nargs={
        {"--version", "-v", &show_version, BOOL, NULL, NULL},
    };

    unnamed_args uargs={
        {&number1, FLOAT, &num1index, &num1success},
        {&number2, FLOAT, &num2index, &num2success},
    };

    i64 num_parsed = parse_arguments(argc-1, argv+1,
            ARRLEN(nargs), nargs,
            ARRLEN(uargs), uargs);

    if (show_version) {
        fprint("%s\n", (fmts){{VERSION}});
        return 1;
    }

    if (num1success && num2success) {
        fprint("%F\n", (fmts){
                {.F = {.val = number1 + number2, .padd = 0, .frac = 8}}
                });
    } else {
        if (!num1success)
            fprint("Invalid floating point number: %s\n", (fmts){{argv[num1index + 1]}});
        if (!num2success)
            fprint("Invalid floating point number: %s\n", (fmts){{argv[num2index + 1]}});

    }

}
