#include "../lib/io.h"

static char* file_buffer[8192];

int main(int argc, char* argv[]) {
    if (argc != 2) {
        puts("Specify Filename\n");
        return 1;
    }
    i64 fd = open(argv[1],0,O_RD);
    if (fd < 0) {
        puts("Failed to open file\n");
        return 2;
    }
    i64 bytes_read = read(fd, file_buffer, sizeof(file_buffer) - 1);
    close(fd);
    write(STDOUT, file_buffer, bytes_read);

    return 0;
}

