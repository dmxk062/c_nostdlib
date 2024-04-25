#include <types.h>
#include <io.h>
#include <args.h>
#include <termcolor.h>

#define VERSION "beta-1.0"

i32 main(i32 argc, zstr argv[]) {

    bool option_set = FALSE;

    named_arguments nargs = {
        {"-c", "--color", &option_set, BOOL, NULL, NULL},
    };

    zstr positional = "Default value";
    bool got_positional = FALSE;
    unnamed_arguments uargs = {
        {&positional, STRING, NULL, &got_positional},
    };

    u64 num_parsed = parse_arguments(argc-1, argv+1,
            ARRLEN(nargs), nargs,
            ARRLEN(uargs), uargs);

    char color[64] = "";
    if (option_set) {
        i64 num_written = gen_ansi_escape(color, 64, (struct AnsiFormat){BLACK, GREEN, TRUE, TRUE, FALSE, FALSE});

    }
    fprint("positional argument: %s%s%s\n", (fmts){
            {color}, {positional}, {ANSI_RESET}
            });
}
