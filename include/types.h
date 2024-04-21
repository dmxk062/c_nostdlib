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

// zero terminated string
typedef char*               zstr;

// might be annoying
typedef void*               untyped;

enum TYPES {
    SINT,
    UINT,
    FLOAT,
    CHAR,
    STRING,
    UNTYPED
};

typedef struct {
    enum TYPES type;
    union {
        i64     i;
        u64     u;
        f64     f;
        char    c;
        char*   s;

        untyped   untyped;
    };
} unitype;

/*
 * better option for type safety
 */

#define DEFRESULT(type, name) \
    typedef struct __result_struct_##name { \
    bool    success; \
    union { \
        u64 errno; \
        type value; \
    }; \
} __result_t_##name; \


#define RESULT(name) \
    __result_t_##name


DEFRESULT(i64, i64);
DEFRESULT(u64, u64);
DEFRESULT(f64, f64);
DEFRESULT(void*, untyped);
DEFRESULT(char*, charptr);
