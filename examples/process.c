#include "../lib/io.h"
#include "../lib/process.h"
#include "../lib/time.h"
#include "../lib/env.h"
#include "../lib/string.h"

int main(int argc, char* argv[]) {

    char* NEW_ARGV[] = {"/bin/sh", "-c", "echo $FOO; trap 'notify-send \"Received SIGTERM\";exit' SIGTERM; while true; do sleep 0.5; done", NULL};
    char format_buffer[1024];

    unsetenv("TERM");
    setenv("FOO", "Hello from raw C!", TRUE);

    i64 pid = fork();
    if (pid == 0) {
        execve("/bin/sh", NEW_ARGV, environ);
    } else {
        fmt("Started new process: %d\n", format_buffer, 1024, pid);
        puts(format_buffer);
        sleep(2, 0, NULL);
        fmt("Sending SIGTERM to %d\n", format_buffer, 1024, pid);
        puts(format_buffer);
        kill(pid, SIGTERM);
    }



    return 0;
}
