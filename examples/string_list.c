#include <types.h>
#include <string.h>
#include <alloc.h>
#include <io.h>
#include <args.h>

i32 main(i32 argc, zstr argv[]) {

    zstr list = "";
    UnnamedArguments uargs = {
        {&list, ArgumentType_STRING},
    };

    parse_arguments(argc-1, argv+1, NULL, NULL, ARRLEN(uargs), uargs);

    String* str = String_new_from_zstr(list).value;
    StringList* strlist = StringList_new(10).value;

    u64 num = String_split_char(str, strlist, ',');
    String* new = StringList_join(strlist, &STRING(" <=> ")).value;
    fprint("%s\n", (fmts){
        {.s = new},
    });

    StringList_free(strlist);
    String_free(str);
    String_free(new);


    return Alloc_get_in_use();
}
