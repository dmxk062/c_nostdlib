#pragma once
#include "../include/types.h"
#include "../alloc/alloc.h"
#include "memcpy.h"



u64 strlen(const char* string);
u64 strcmp(const char* string1, const char* string2);
u64 strncmp(const char* string1, const char* string2, u64 len);

char* strdup(const char* str);


typedef struct {
    f64 val;
    u64 padd;
    u64 frac;
} float_fmt;

typedef struct {
    i64 val;
    u64 padd;
} int_fmt;

typedef struct {
    char* val;
    u64 padd;
} str_fmt;

typedef struct fmt_value {
    union  {
        char*       s; // unformatted string
        str_fmt     S; // formatted string
        i64         i; // unformatted int
        int_fmt     I; // formatted int
        f64         f; // unformatted float 
        float_fmt   F; // formatted float
        u8          c;
    };
} fmt_value;

typedef fmt_value fmts[];
i64 fmt(const char* format, char* out, u64 outlen, fmts values);

