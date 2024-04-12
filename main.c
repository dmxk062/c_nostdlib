#include "lib/io.h"
#include "lib/syscall.h"
#include "lib/string.h"
#include "alloc/alloc.h"


int main(int argc, char* argv[]) {
    char* tmp=malloc(4050);
    char* tmp2=malloc(4050);
    char* tmp3=malloc(4050);

    puts("1\n");
    free(tmp);

    puts("2\n");
    free(tmp2);


    puts("3\n");
    free(tmp3);
    return 0;
}
