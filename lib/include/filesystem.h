#pragma once
#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

#include "types.h"

errno_t unlink(const zstr path);
errno_t access(const zstr path, u64 mode);
errno_t chdir(const zstr path);
errno_t getcwd(char* buf, u64 size);



enum AccessMode {
    AccessMode_R = 4,
    AccessMode_W = 2,
    AccessMode_X = 1,
    AccessMode_F = 0,
};

#endif
