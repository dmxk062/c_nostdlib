#include <types.h>
#include <io.h>
#include <args.h>

#define VERSION "beta-1.0"

i32 main(i32 argc, zstr argv[]) {

    bool option_set = FALSE;

    named_arguments nargs = {
        {"-o", "--option", &option_set, BOOL, NULL, NULL},
    };

    zstr positional = NULL;
    bool got_positional = FALSE;
    unnamed_arguments uargs = {
        {&positional, STRING, NULL, &got_positional},
    };

    u64 num_parsed = parse_arguments(argc-1, argv+1,
            ARRLEN(nargs), nargs,
            ARRLEN(uargs), uargs);

    if (option_set) {
        print("-o/--option was set\n");
    }
    if (got_positional) {
        fprint("positional argument: %s\n", (fmts){{positional}});
    }
}
