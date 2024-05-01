#pragma once

#ifndef _CSTRING_H
#define _CSTRING_H

#include <types.h>



u64 strlen(const char* string);
u64 strcmp(const char* string1, const char* string2);
bool streq(const char* string1, const char* string2);
u64 strncmp(const char* string1, const char* string2, u64 len);
i64 strfind(const char* string, u64 len, char c);

char* strdup(const char* str);

#endif
