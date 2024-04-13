#include "string.h"
#include "memcpy.h"
#include "../alloc/alloc.h"
#include "../include/types.h"

extern char** environ;

char* getenv(const char* name);
i64   setenv(const char* name, const char* value, bool replace);
