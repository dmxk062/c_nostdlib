#pragma once
#ifndef _FORMAT_H
#define _FORMAT_H


#include <types.h>
#include <termcolor.h>
#include <string.h>

/*
 * Parameters for fmt()
 * TODO?: move to u8 or u16 for some fields to save space
 * TODO?: add option to replace decimal separator
 * TODO?: add more types, e.g. f128 or u8, rn they can just be cast to f64 and i64
 */

// a formatted float
typedef struct {
    f128 val;
    u16 padd;
    u16 frac;
} float_fmt;


// a formatted int
typedef struct {
    i64  val;
    u16 padd;
} int_fmt;

// a formatted null terminated string
typedef struct {
    zstr val;
    u16 padd;
} zstr_fmt;

// a formatted string
typedef struct {
    String* val;
    u16 padd;
} string_fmt;

/*
 * The capital versions offer additional formatting options 
 */
typedef union  {
        untyped     any;
        // null terminated char*
        zstr        z;
        zstr_fmt    Z;

        // string object
        String*     s;
        string_fmt  S;

        // ansi color escape code
        struct AnsiFormat e;
        i64         i;
        int_fmt     I;
        f128        f;
        float_fmt   F;
        u8          c;
} fmt_value;
typedef fmt_value fmts[];




Result(u64) fmt(const zstr format, char* out, u64 outlen, fmts values);
Result(u64) String_format(String* str, const zstr format, fmt_value values[]);

#define Fmt(...) (fmt_value[]){__VA_ARGS__}

#endif
