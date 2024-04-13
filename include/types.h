// vim: ft=c
#pragma once

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

typedef signed long int     i64;
typedef unsigned long int   u64;

typedef signed int          i32;
typedef unsigned int        u32;

typedef signed short        i16;
typedef unsigned short      u16;

typedef signed char         i8;
typedef unsigned char       u8;


typedef long double         f128;
typedef double              f64;
typedef float               f32;

/*
 * Functions should generally return either i64 or bool to indicate success
 * bool if there only are two states and no additional information would be useful
 * i64  if there is some additional information (= error code) the function wants to communicate
 */
typedef u8                  bool;


#define NULL        0
#define TRUE        1
#define FALSE       0
