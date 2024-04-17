#pragma once
#include "../include/types.h"
#include "../alloc/alloc.h"
#include "memcpy.h"



u64 strlen(const char* string);
u64 strcmp(const char* string1, const char* string2);
u64 strncmp(const char* string1, const char* string2, u64 len);

char* strdup(const char* str);




/*
 * Parameters for fmt()
 * TODO?: move to u8 or u16 for some fields to save space
 * TODO?: add option to replace decimal separator
 * TODO?: add more types, e.g. f128 or u8, rn they can just be cast to f64 and i64
 */

typedef struct {
    f64 val;
    u16 padd;
    u16 frac;
} float_fmt;

typedef struct {
    i64 val;
    u16 padd;
} int_fmt;

typedef struct {
    char* val;
    u16 padd;
} str_fmt;

/*
 * The capital versions offer additional formatting options 
 */
typedef union  {
        char*       s;
        str_fmt     S;
        i64         i;
        int_fmt     I;
        f64         f;
        float_fmt   F;
        u8          c;
} fmt_value;

typedef fmt_value fmts[];
i64 fmt(const char* format, char* out, u64 outlen, fmts values);


