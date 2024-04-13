#include "../lib/string.h"
#include "../lib/stat.h"
#include "../alloc/alloc.h"
#include "../lib/io.h"

int main(int argc, char* argv[]) {
    
    if (argc < 2) {
        puts("specify at least one file or directory\n");
        return 1;
    }


    const i64 text_buffer_size = 4096;
    char* text_buffer = malloc(text_buffer_size);
    if (text_buffer == NULL) {
        puts("Failed to allocate memory\n");
    }
    struct stat st;

    for (i64 i = 1; i < argc; i++) {
        i64 ret = stat(argv[i], &st);
        if (ret < 0) {
            i64 ret = fmt("Failed to stat %s\n", text_buffer, text_buffer_size, argv[i]);
            if (ret > 0) {
                puts(text_buffer);
            }
            continue;
        }

        ret = fmt("%s:\nUser:  %$d\nGroup: %$d\nSize:  %d\nPerms: %$o\n", text_buffer, text_buffer_size, 
                argv[i], 4, st.st_uid, 4, st.st_gid, st.st_size, 4, st.st_mode & 0777);
        if (ret > 0)
            puts(text_buffer);

    }

    return 0;
}
