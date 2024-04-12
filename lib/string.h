#pragma once
#include "io.h"
#include "../alloc/alloc.h"
#include "memcpy.h"


u64 strlen(const char* string);
u64 strcmp(const char* string1, const char* string2);

char* u64_to_hex(u64 num);
char* u32_to_hex(u64 num);
// char* u_to_hex(void* num, u64 num_digits);

i64 strformat(const char* format, char* output, u64 len, char** strings);
