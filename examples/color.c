#include <types.h>
#include <io.h>
#include <args.h>
#include <termcolor.h>


i32 main(i32 argc, zstr argv[]) {

    bool color_red  = FALSE;
    bool color_blue = FALSE;

    NamedArguments nargs = {
        {"-r", "--red", &color_red,   ArgumentType_BOOL},
        {"-b", "--blue", &color_blue, ArgumentType_BOOL}
    };

    zstr positional = NULL;
    bool got_positional = FALSE;
    UnnamedArguments uargs = {
        {&positional, ArgumentType_STRING, NULL, &got_positional}
    };

    parse_arguments(argc-1, argv+1,
            ARRLEN(nargs), nargs,
            ARRLEN(uargs), uargs);

    enum AnsiColor color = AnsiColor_NONE;

    if (color_red && color_blue)
        color = AnsiColor_MAGENTA;
    else if (color_red) 
        color = AnsiColor_RED;
    else if (color_blue)
        color = AnsiColor_BLUE;

    if (got_positional) {
        fprint("%e%z%E\n", (fmts){
            {.e = {.fg = AnsiColor_BLACK, .bg = color}}, 
            {.z = positional}
        });
    }

}
