#include "include/types.h"
#include "lib/io.h"
#include "lib/stat.h"
#include "lib/syscall.h"
#include "lib/string.h"
#include "alloc/alloc.h"


int main(int argc, char* argv[]) {
    
    struct stat st;
    i64 ret = stat(argv[1], &st);
    //
    char* hex_len = u32_to_hex(st.st_gid);
    puts(hex_len);
    puts("\n");
    free(hex_len);

    return 0;
}
