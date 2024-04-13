#include "lib/io.h"
#include "lib/process.h"
#include "lib/env.h"

int main(int argc, char* argv[]) {

    unsetenv("TERM");
    execve("/bin/bash", NULL, environ);



    return 0;
}
