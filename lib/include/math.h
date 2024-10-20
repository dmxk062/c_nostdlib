#pragma once
#ifndef _MATH_H
#define _MATH_H

#include <types.h>

#define NAN (0.0 / 0.0)
#define ISNAN(x) ((x) != (x))
#define ABS(x) ((x) < 0 ? -(x) : (x))



f128 fabs(f128 x);
u64 abs(i64 x);
f128 pow(f128 base, i64 exponent);
f128 root(f128 number, u64 exponent);
#endif
