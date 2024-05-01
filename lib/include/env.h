#pragma once
#ifndef _ENV_H
#define _ENV_H


#include <types.h>

extern zstr* environ;

RESULT(zstr) getenv(const char* name);
errno_t setenv(const char* name, const char* value, bool replace);
errno_t unsetenv(const char* name);
#endif
