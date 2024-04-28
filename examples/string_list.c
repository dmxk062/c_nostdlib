#include <types.h>
#include <string.h>
#include <alloc.h>
#include <io.h>
#include <args.h>

i32 main(i32 argc, zstr argv[]) {

    zstr list = "";
    UnnamedArguments uargs = {
        {&list, ARGSTRING},
    };

    parse_arguments(argc-1, argv+1, NULL, NULL, ARRLEN(uargs), uargs);

    String str = string_new_from_zstr(list).value;
    StringList* strlist = string_list_new(10).value;

    u64 num = string_split_char(str, strlist, ',');
    String new = string_list_join(strlist, STRING(" <=> ")).value;
    fprint("%s\n", (fmts){
        {.s = new},
    });

    string_list_free(strlist);
    string_free(str);
    string_free(new);


    return get_used_chunk_count();
}
