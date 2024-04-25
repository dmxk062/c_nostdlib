#include <types.h>
#include <string.h>
#include <io.h>
#include <parsers.h>
#include <math.h>
#include <args.h>
#include <format.h>

i32 main(i32 argc, zstr argv[]) {

    bool option = FALSE;
    zstr value = NULL;
    zstr name  = NULL;

    named_args nargs={
        {"-v", "--value",  &value, STRING},
        {"-o", "--option", &option, BOOL},
    };
    unnamed_args uargs={
        {&name, STRING},
    };
    i64 parsed_args = parse_arguments(argc-1, argv+1,
            ARRLEN(nargs), nargs,
            ARRLEN(uargs), uargs
            );

    if (option){
        print("--option/-o was set!\n");
    }
    if (value){
        fprint("--value/-v was set to %s\n", (fmts){{value}});
    }
    if (name){
        fprint("got positional argument: %s\n", (fmts){{name}});
    }

    fprint("Got %d arguments in total, parsed %d successfully\n", (fmts){
            {.i=argc-1}, {.i=parsed_args}
            });


}
