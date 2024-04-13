#pragma once

#include "syscall.h"
#include "../include/types.h"

i64 ioctl(u64 fd, u64 cmd, void* arg);
