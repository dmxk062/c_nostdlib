# define NULL 0
#include "io.h"
#include "syscall.h"





static const char ERROR_ARGS[] = "Specify exactly two arguments\n";
static const char ERROR_OPEN[] = "Failed to open file\n";

int main(int argc, char* argv[]) {
    static char buffer[4096];
    if (argc != 2) {
        write(STDERR, ERROR_ARGS, sizeof(ERROR_ARGS));
        exit(1);
    }
    i64 fd=open(argv[1], 0, O_RD);
    if (fd < 0) {
        write(STDERR, ERROR_OPEN, sizeof(ERROR_OPEN));
        exit(2);
    }
    i64 num_read=read(fd, buffer, sizeof(buffer));
    close(fd);
    write(1, buffer, num_read);
    return 0;
}
