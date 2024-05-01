#pragma once
#ifndef _ENV_H
#define _ENV_H


#include <types.h>

extern char** environ;

char* getenv(const char* name);
i64   setenv(const char* name, const char* value, bool replace);
i64   unsetenv(const char* name);
#endif
