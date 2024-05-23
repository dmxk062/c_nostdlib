// vim: ft=c
#pragma once
#ifndef _TYPES_H
#define _TYPES_H


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
#define true                (bool)1
#define false               (bool)0

// zero terminated string
typedef char*               zstr;

// might be annoying
typedef void*               untyped;

// error number
typedef u64                 errno_t;

#ifndef Result
/*
 * better option for type safety
 */
#define DefineResult(type, name) \
    typedef struct __result_struct_##name { \
    bool    ok; \
    union { \
        u64 errno; \
        type value; \
    }; \
} __result_t_##name; \

/*
 * general naming convention:
 * for regular scalar types or typedef'd structs/unions: just the type name
 * for pointers <type>ptr
 */
#define Result(name) \
    __result_t_##name

#define Ok(type, val) \
    ((Result(type)){true, .value = (val)})

#define Err(type, err) \
    ((Result(type)){false, .errno = (err)})


#endif /* Result */

#ifndef DoWhileSuccessful

/*
 * Perform an action until the Result() returned is not successful
 */
#define DoWhileSuccessful(var, funct) \
    for(var = funct; var.ok; var = funct)

#endif 

#ifndef ARRLEN
/*
 * get the number of items in a static array
 */
#define ARRLEN(array) \
    sizeof(array) / sizeof(array[0])

#endif /* ARRLEN */

#ifndef COUNTED_ARRAY

#define COUNTED_ARRAY(type, name) \
    struct { \
        u64     size; \
        u64     len; \
        type    name; \
    }

#endif /* COUNTED_ARRAY */

DefineResult(i64, i64);
DefineResult(u64, u64);
DefineResult(f64, f64);
DefineResult(f128, f128);
DefineResult(untyped, untyped);
DefineResult(zstr,  zstr);

#endif
