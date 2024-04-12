#include "lib/string.h"
#include "alloc/alloc.h"
#include "lib/io.h"

int main(int argc, char* argv[]) {
    
    char* name = strdup(argv[0]);
    puts(name);
    free(name);
    return 0;
}
