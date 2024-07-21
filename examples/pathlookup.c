#include <constants.h>
#include <cstring.h>
#include <env.h>
#include <utils/path.h>
#include <io.h>
#include <string.h>
#include <types.h>
#include <filesystem.h>

#define PATH_MAX_LEN 64

i32 main(i32 argc, zstr argv[]) {
    Result(zstr) new_execpath = getenv("PATH");
    if (!new_execpath.ok) {
        print("$PATH is not set\n");
        return 1;
    }

    PResult(String) new_path_string = String_new_from_zstr(new_execpath.value);
    PResult(String) new_result_path = String_new(PATH_MAX);
    PResult(StringList) new_path_list = StringList_new(PATH_MAX_LEN);
    if (!new_path_string.ok || !new_path_list.ok || !new_result_path.ok) {
        print("Failed to allocate ram\n");
        return 2;
    }


    StringList* path_list = new_path_list.value;

    u64 num_elements = String_split_char(new_path_string.value, path_list, ':');

    errno_t err;
    String* output_path = new_result_path.value;
    struct PathAccess pathstruct = {.path = path_list};
    for (u64 i = 1; i < argc; i++) {
        err = find_in_path(&pathstruct,
                output_path, 
                &(String){.buffer = argv[i], .len = strlen(argv[i])},
                AccessMode_X|AccessMode_F
        );
        if (err) {
            fprint("Failed to find %z in $PATH\n", (fmts){{.z = argv[i]}});
            continue;
        }
        fprint("%s\n", (fmts){{.s = output_path}});
    }

    String_free(new_path_string.value);
    String_free(output_path);
    StringList_free(path_list);

    return 0;
}
