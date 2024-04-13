#include "lib/io.h"
#include "lib/process.h"
#include "lib/time.h"
#include "lib/env.h"
#include "lib/string.h"

int main(int argc, char* argv[]) {

    char* NEW_ARGV[] = {"/bin/sh", NULL};

    unsetenv("TERM");
    setenv("FOO", "Hello from raw C!", TRUE);

        execve("/bin/sh", NEW_ARGV, environ);


    return 0;
}
