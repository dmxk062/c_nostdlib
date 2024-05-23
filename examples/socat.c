#include <types.h>
#include <cstring.h>
#include <errno.h>
#include <socket.h>
#include <io.h>
#include <args.h>

i32 main(i32 argc, zstr argv[]) {

    i64 num_to_read = 0;
    bool got_num_to_read = false;
    NamedArguments nargs = {
        {"-c", "--count", &num_to_read, ArgumentType_INT, NULL, &got_num_to_read},
    };

    zstr path;
    bool got_path = false;
    UnnamedArguments uargs = {
        {&path, ArgumentType_STRING, NULL, &got_path},
    };

    u64 num_parsed = parse_arguments(argc-1, argv+1,
            ARRLEN(nargs), nargs,
            ARRLEN(uargs), uargs);


    if (!got_path) {
        fwrite(STDERR, "%z: PATH\n", (fmts){{.z = argv[0]}});
        return 1;
    }

    Result(u64) new_sockfd = Socket_new_UnixClient(path, strlen(path));

    if (!new_sockfd.ok) {
        char* err_msg = ERROR_MESSAGES[new_sockfd.errno];
        if (err_msg == NULL)
            err_msg = "Unknown error";

        fwrite(STDERR, "Failed to connect to %z: %z (errno: %d)\n",(fmts){
            {.z = path}, {.z = err_msg}, {.i = new_sockfd.errno}
        });
        return 1;
    }

    u64 stop_after = 0;
    u64 num_read = 0;
    if (got_num_to_read)
        stop_after = (u64) num_to_read;
        


    u64 fd = new_sockfd.value;
    Result(u64) new_bytes_read;
    char buffer[1024];
    WHILE_SUCCESS(new_bytes_read, Socket_recv(fd, buffer, 1024, NULL)) {
        num_read++;
        if (new_bytes_read.value == 0)
            break;
        write(STDOUT, buffer, new_bytes_read.value);
        if (got_num_to_read && (num_read == stop_after)) {
            break;
        }
    }

    close(fd);

    return 0;
}
