#pragma once
#ifndef _ENVIRON_H
#define _ENVIRON_H
#include "types.h"

/*
 * Environment handlign
 * This is a safer, heap based alternative to the env.h header
 * Instead of modifying the stack directly, this copies the environment to the heap and modifies it there
 * This allos this to free allocated environment variables
 */


Result(zstr) getenv(const zstr name);
errno_t setenv(const zstr name, const zstr value, bool replace);
errno_t unsetenv(const zstr name);






#endif
