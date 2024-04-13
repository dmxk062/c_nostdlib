#include "lib/io.h"

extern char** environ;
int main(int argc, char* argv[]) {

    puts(environ[0]);


    return 0;
}
