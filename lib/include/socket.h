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
    char            path[UNIX_PATH_MAX];
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


errno_t socket_connect(u64 fd, struct SocketAddress* addr, u64 address_length);

RESULT(u64) socket_new_unix(char* path, u64 path_length);
