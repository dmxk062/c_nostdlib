#include <cstring.h>
#include "signal.h"
#include "filesystem.h"
#include "types.h"
#include <errno.h>
#include <stat.h>
#include <socket.h>
#include <argument.h>
#include <io.h>


volatile bool received_interrupt = false;


void handle_interrupt(i64 interrupt) {
    received_interrupt = true;
    return;
}

i32 main(i32 argc, zstr argv[]) {

    zstr socket_path = "/tmp/nostd.sock";
    bool delete_old = false;
    ArgDesc_Named nargs[] = {
        {&socket_path, ArgumentType_STRING, 'p', "path", "PATH", "Path for the socket"},
        {&delete_old, ArgumentType_SET, 'd', "delete", NULL, "Delete old sockets first"},
    };

    zstr message = "Default message";
    ArgDesc_Positional uargs[] = {
        {&message, ArgumentType_STRING, "MESSAGE", "The message to send over the socket", false},
    };

    errno_t err = Arguments_parse(argc, argv,
            ARRLEN(nargs), nargs,
            ARRLEN(uargs), uargs,
            "socket_server", "Listen on PATH or /tmp/nostd.sock and send MESSAGE to clients", "");
    if (err) { return err; }

    if (delete_old) {
        struct Stat st;
        errno_t err = stat(socket_path, &st);
        if (!err && st.mode.type == S_SOCK)
            unlink(socket_path);
    }

    Result(u64) new_server_fd = Socket_new_UnixServer(socket_path, strlen(socket_path), 20);
    if (!new_server_fd.ok) {
        zstr err_msg = ERROR_MESSAGES[new_server_fd.errno];
        if (err_msg == NULL)
            err_msg = "Unknown error";
        fwrite(STDERR, "Failed to create server socket %z: %z (errno %d)\n",
            Fmt({.z = socket_path}, {.z = err_msg}, {.i = new_server_fd.errno})
        );
        return 1;
    }

    Signal_action(Signal_INT, handle_interrupt, 0, 0, NULL);


    u64 server_fd = new_server_fd.value;

    Result(u64) new_client_fd;
    struct SocketAddress client_addr;
    u64 client_addr_length = 0;

    fprint("Listening on %z (fd %d)\n", (fmts){{.z = socket_path}, {.i = server_fd}});

    u64 num_connections = 0;
    while (!received_interrupt) {
        new_client_fd = Socket_accept(server_fd, &client_addr, &client_addr_length);
        if (!new_client_fd.ok)
            continue;

        fprint("New connection on fd %d\n", (fmts){{.i = new_client_fd.value}});
        num_connections++;
        fwrite(new_client_fd.value, "Connection number %d: %z\n", (fmts){{.i = num_connections}, {.z = message}});
        close(new_client_fd.value);
    }

    close(server_fd);
    unlink(socket_path);
    return 0;
}
