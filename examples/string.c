#include <types.h>
#include <string.h>
#include <alloc.h>
#include <io.h>
#include <args.h>

i32 main(i32 argc, zstr argv[]) {

    zstr to_index = "This is some example text";
    i64  count  = 0;
    bool got_string = FALSE;
    bool got_count  = FALSE;
    UnnamedArguments uargs = {
        {&to_index, ArgumentType_STRING, NULL, &got_string},
        {&count,    ArgumentType_INT,    NULL, &got_count},
    };

    u64 num_parsed = parse_arguments(argc-1, argv+1,
            NULL, NULL,
            ARRLEN(uargs), uargs);

    String argument = String_new_from_zstr(to_index).value;

    RESULT(String) sliced = String_slice(argument, 0, count);
    if (!sliced.success) {
        fwrite(STDERR, "`%s` is shorter than %d\n", (fmts){
            {.s = argument}, {.i = count}
        });
        String_free(argument);
        return 1;
    }

    String_free(argument);
    fprint("%s\n", (fmts){
        {.s = sliced.value}
    });

    String_free(sliced.value);



    return Malloc_get_used_count();
}
