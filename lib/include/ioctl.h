#pragma once

#include <types.h>

RESULT(u64) ioctl(u64 fd, u64 cmd, void* arg);

// TODO: add macros for commands
