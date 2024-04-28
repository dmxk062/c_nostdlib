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
    unnamed_arguments uargs = {
        {&to_index, ARGSTRING, NULL, &got_string},
        {&count,    ARGINT,    NULL, &got_count},
    };

    u64 num_parsed = parse_arguments(argc-1, argv+1,
            NULL, NULL,
            ARRLEN(uargs), uargs);

    string argument = string_new_from_zstr(to_index).value;

    RESULT(string) sliced = string_slice(argument, 0, count);
    if (!sliced.success) {
        fwrite(STDERR, "`%s` is shorter than %d\n", (fmts){
            {.s = argument}, {.i = count}
        });
        string_free(argument);
        return 1;
    }

    string_free(argument);
    fprint("%s\n", (fmts){
        {.s = sliced.value}
    });

    string_free(sliced.value);



    return get_used_chunk_count();
}
