#include <types.h>
#include <args.h>
#include <io.h>

i32 main(i32 argc, zstr argv[]) {

    bool show_help    = false;
    NamedArguments nargs = {
        {"-h", "--help", &show_help, ArgumentType_BOOL},
    };

    u64 num_parsed = parse_arguments(argc - 1, argv + 1, ARRLEN(nargs), nargs, 0, NULL);

    if (show_help) {
        fprint("%z: [OPTIONS]\n"
               " -h, --help     Show this page\n",
            (fmts){
            {.z = argv[0]}
        });
        return 1;
    }

    print("Hello World\n");

    return 0;
}
