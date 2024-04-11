#include "lib/io.h"
#include "lib/string.h"
#include "alloc/alloc.h"


int main(int argc, char* argv[]) {
    char* hex1 = u64_to_hex(128);
    char* hex2 = u64_to_hex(128);
    char* buffer = malloc(1024);
    char* fmt[2];
    fmt[0]=hex1;
    fmt[1]=hex2;
    strformat("0x%\n0x%\n", buffer, 1023, fmt);
    puts(buffer);

    return 0;
}
