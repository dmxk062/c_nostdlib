#pragma once
#include "../include/types.h"
#include "../alloc/alloc.h"
#include "memcpy.h"



u64 strlen(const char* string);
u64 strcmp(const char* string1, const char* string2);
u64 strncmp(const char* string1, const char* string2, u64 len);

char* strdup(const char* str);




