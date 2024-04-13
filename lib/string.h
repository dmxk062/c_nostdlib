#pragma once
#include "../include/types.h"


u64 strlen(const char* string);
u64 strcmp(const char* string1, const char* string2);


char* strdup(const char* str);

i64 fmt(const char* format, char* out, u64 outlen, ...);
