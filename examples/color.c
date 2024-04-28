#include <types.h>
#include <io.h>
#include <args.h>
#include <termcolor.h>


i32 main(i32 argc, zstr argv[]) {

    bool color_red  = FALSE;
    bool color_blue = FALSE;

    named_arguments nargs = {
        {"-r", "--red", &color_red,   ARGBOOL},
        {"-b", "--blue", &color_blue, ARGBOOL}
    };

    zstr positional = NULL;
    bool got_positional = FALSE;
    unnamed_arguments uargs = {
        {&positional, ARGSTRING, NULL, &got_positional}
    };

    i64 num_parsed = parse_arguments(argc-1, argv+1,
            ARRLEN(nargs), nargs,
            ARRLEN(uargs), uargs);

    enum AnsiColor color = CNONE;

    if (color_red && color_blue)
        color = CMAGENTA;
    else if (color_red) 
        color = CRED;
    else if (color_blue)
        color = CBLUE;

    if (got_positional) {
        fprint("%e%z%E\n", (fmts){
            {.e = {.fg = CBLACK, .bg = color}}, 
            {.z = positional}
        });
    }

}
