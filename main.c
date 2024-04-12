#include "lib/io.h"
#include "lib/syscall.h"
#include "lib/string.h"
#include "alloc/alloc.h"


int main(int argc, char* argv[]) {
    char* buffer = malloc(4096);

    char* hex1 = u64_to_hex(128);
    char* hex2 = u64_to_hex(1024 * 1024 * 1024);
    char* fmt[] = {hex1, hex2};

    i64 num_written = strformat("128:    0x%\n1024^3: 0x%\n", buffer, 4096, fmt);

    puts(buffer);
    return 0;
}
