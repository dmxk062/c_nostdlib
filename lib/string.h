#pragma once
#include "io.h"
#include "../alloc/alloc.h"
#include "memcpy.h"


u64 strlen(const char* string);
u64 strcmp(const char* string1, const char* string2);

char* u64_to_hex(u64 num);

i64 strformat(const char* format, char* output, u64 len, char** strings);
