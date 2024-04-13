#include "lib/io.h"
#include "lib/env.h"

int main(int argc, char* argv[]) {

    char* shell = getenv("SHELL");
    if (shell != NULL) {
        puts(shell);
    }




    return 0;
}
