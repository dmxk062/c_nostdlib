// vim: ft=c
#pragma once

#define NULL        0

// no use for them right now
// typedef signed __int128     i128;
// typedef unsigned __int128   u128;
//

/*
 * Type naming convention: <type><size>
 * where type is:
 *      - i : signed integer
 *      - u : unsigned integer
 *      - f : float
 */


// 64 bit signed integer
typedef signed long int     i64;
// 64 bit unsigned integer
typedef unsigned long int   u64;

// 32 bit signed integer
typedef signed int          i32;
// 32 bit unsigned integer
typedef unsigned int        u32;

// 16 bit signed integer
typedef signed short        i16;
// 32 bit unsigned integer
typedef unsigned short      u16;

// 8 bit signed integer
typedef signed char         i8;
// 8 bit unsigned integer
typedef unsigned char       u8;


typedef long double         f128;
typedef double              f64;
typedef float               f32;

typedef _Bool               bool;
#define TRUE                1
#define FALSE               0

/*
 * Return value for a function
 * Check for errors before using value or errno
 */
typedef struct {
    bool        success;
    union {
        void*   value;
        u64     errno;
    };
} result;



// zero terminated string
typedef char*               zstr;

// might be annoying
typedef void*               untyped;
