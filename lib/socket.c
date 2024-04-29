#include <constants.h>
#include <memcpy.h>
#include <syscall.h>
#include <types.h>
#include <socket.h>
#include <errno.h>

RESULT(u64) socket_new(enum SocketFamily family, enum SocketDomain domain, i64 protocol) {
    i64 ret = (i64)syscall3(SYS_SOCKET,
            (untyped)family,
            (untyped)domain,
            (untyped)protocol);

    if (ret < 0) 
        return (RESULT(u64)){.success = FALSE, .errno = -ret};
    else
        return (RESULT(u64)){.success = TRUE, .value = ret};
}

errno_t socket_connect(u64 fd, struct SocketAddress* addr, u64 address_length) {
    return (u64) -(i64)syscall3(SYS_CONNECT,
            (untyped)fd,
            (untyped)addr,
            (untyped)address_length);
}

RESULT(u64) socket_new_unix(char* path, u64 path_length) {
    if (path_length > UNIX_PATH_MAX)
        return (RESULT(u64)){.success = FALSE, .errno = ENAMETOOLONG};

    RESULT(u64) new_sockfd = socket_new(AF_UNIX, SOCK_STREAM, 0);
    if (!new_sockfd.success)
        return new_sockfd;


    struct SocketAddressUnix addr;
    addr.family = AF_UNIX;
    memcpy(addr.path, path, path_length);

    errno_t errno = socket_connect(new_sockfd.value, (struct SocketAddress*)&addr, path_length);
    if (errno) {
        return (RESULT(u64)){.success = FALSE, .errno = errno};
    }

    return (RESULT(u64)){.success = TRUE, .value = new_sockfd.value};

}

RESULT(u64) socket_recv(u64 fd, void* buffer, u64 count, u64 flags) {
    i64 ret = (i64)syscall4(SYS_RECVFROM,
            (untyped)fd,
            buffer,
            (untyped)count,
            (untyped)flags);
    if (ret < 0) 
        return (RESULT(u64)){.success = FALSE, .errno = -ret};
    else
        return (RESULT(u64)){.success = TRUE, .value = ret};
}
