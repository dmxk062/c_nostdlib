#pragma once
#ifndef _PARSER_H
#define _PARSER_H

#include <types.h>

i64 char_to_int(const char c);

RESULT(i64) str_to_int(const char* string, u64 n, u64 base);

RESULT(f128) str_to_float(const char* string, u64 n);
#endif 
