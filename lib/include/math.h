#pragma once
#include "types.h"

#define NAN (0.0 / 0.0)
#define ISNAN(x) ((x) != (x))



f128 fabs(f64 x);
u64 abs(i64 x);
f128 pow(f128 base, i64 exponent);
f128 root(f128 number, u64 exponent);
