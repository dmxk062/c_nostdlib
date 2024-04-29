#pragma once
#include <types.h>
#include <constants.h>

typedef u16 socket_family_t;

struct SocketAddress {
    socket_family_t family;
    u8              data[14];
};

struct SocketAddressUnix {
    socket_family_t family;
    char   path[UNIX_PATH_MAX];
};

enum SocketFamily {
    AF_UNIX = 1,
};

enum SocketDomain {
    SOCK_STREAM = 1,
    SOCK_DGRAM  = 2,
    SOCK_RAW    = 3,
    SOCK_RDM    = 4,
};

RESULT(u64) socket_new(enum SocketFamily family, enum SocketDomain domain, i64 protocol);
RESULT(u64) socket_recv(u64 fd, void* buffer, u64 count, u64 flags);
RESULT(u64) socket_send(u64 fd, void* buffer, u64 count, u64 flags);

errno_t socket_bind(u64 fd, struct SocketAddress* peer, u64 peer_length);
RESULT(u64) socket_accept(u64 fd, struct SocketAddress* addr, u64* addr_length);

enum SocketShutdown {
    SOCK_SHUTDOWN_RD   = 0,
    SOCK_SHUTDOWN_WR   = 1,
    SOCK_SHUTDOWN_RDWR = 2,
};
errno_t socket_shutdown(u64 fd, enum SocketShutdown how);
errno_t socket_connect(u64 fd, struct SocketAddress* addr, u64 addr_length);
errno_t socket_listen(u64 fd, u64 backlog);

RESULT(u64) socket_new_unix(char* path, u64 path_length);

/*
 * Flags
 */

#define MSG_OOB             0x1
#define MSG_PEEK            0x2
#define MSG_DONTROUTE       0x4
#define MSG_MORE            0x8000
