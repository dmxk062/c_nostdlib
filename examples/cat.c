#include "../lib/io.h"
#include "../lib/string.h"
#include "../alloc/alloc.h"


#define BUFFER_SIZE 1024 * 4

static const char USAGE_STR[] = ": FILES\n";
static const char OPEN_ERROR_STR[] = "Failed to open file: ";
static const char* NEWLINE = "\n";


i64 cat_file(char* path) {
    i64 fd = open(path, NULL, O_RD);
    u64 num_read, total_read;
    if (fd < 0) {
        return -1;
    }
    char* buffer = malloc(BUFFER_SIZE);
    if (buffer == NULL) {
        return -1;
    }

    while (TRUE) {
        num_read = read(fd, buffer, BUFFER_SIZE);
        total_read+=num_read;

        if (num_read == 0) {
            break;
        }

        write(STDOUT, buffer, num_read);
    }

    free(buffer);
    return num_read;
}

int main(int argc, char* argv[]) {

    i64 ret;
    if (argc < 2) {
        write(STDERR, argv[0], strlen(argv[0]));
        write(STDERR, USAGE_STR, sizeof(USAGE_STR));
        return 1;
    }

    for (i64 i = 1; i < argc; i++) {
        ret = cat_file(argv[i]);
        if (ret < 0) {
            write(STDERR, OPEN_ERROR_STR, sizeof(OPEN_ERROR_STR));
            write(STDERR, argv[i], strlen(argv[i]));
            write(STDERR, NEWLINE, 1);
        }

    }



}
