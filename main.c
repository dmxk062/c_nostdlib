#include "lib/io.h"
#include "alloc/alloc.h"


int main(int argc, char* argv[]) {
    if (argc != 2) {
        return 1;
    }
    i64 fd = open(argv[1], 0, O_RD);
    if (fd < 0) {
        return 2;
    }
    char* buf = malloc(4100);
    u64 n = read(fd, buf, 4095);
    buf[n+1] = '\0';
    puts(buf);
    // free(buf);
    char* newbuf = malloc(90000);
    return 0;
}
