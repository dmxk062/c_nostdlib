#pragma once
#include "types.h"
#ifndef _FCNTL_H
#define _FCNTL_H

enum FcntlOp {
    FnctlOp_DUPFD  = 0,
    FcntlOp_GETFD  = 1,
    FcntlOp_SETFD  = 2,
    FcntlOp_GETFL  = 3,
    FcntlOp_SETFL  = 4,
    FcntlOp_GETLK  = 5,
    FcntlOp_SETLK  = 6,
    FcntlOp_SETLKW = 7,

    FcntlOp_SETOWN = 8,
    FcntlOp_GETOWN = 9,
};

RESULT(u64) fcntl(u64 fd, enum FcntlOp op, u64 arg);

#endif
