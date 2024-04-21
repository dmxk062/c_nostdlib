#pragma once
#include "../include/types.h"

#define NAN (0.0 / 0.0)
#define ISNAN(x) ((x) != (x))



f64 fabs(f64 x);
u64 abs(i64 x);
f64 pow(f64 base, i64 exponent);
f64 root(f64 number, i64 exponent);
