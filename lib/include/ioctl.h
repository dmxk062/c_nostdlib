#pragma once
#ifndef _IOCTL_H
#define _IOCTL_H


#include <types.h>

Result(u64) ioctl(u64 fd, u64 cmd, void* arg);

// TODO: add macros for commands
#endif 
