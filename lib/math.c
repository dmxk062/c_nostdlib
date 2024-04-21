#include "math.h"

f64 fabs(f64 x) {
    return x < 0 ? -x : x;
}

u64 abs(i64 x) {
    return x < 0 ? -x : x;
}

f64 pow(f64 base, i64 exponent) {
    f64 result = 1.0;
    i64 i;

    for (i = 0; i < exponent; ++i) {
        result *= base;
    }
    
    return result;
}

f64 root(f64 number, i64 exponent) {
    if (number < 0 && exponent % 2 == 0) {
        return NAN;
    }

    f64 guess = number / 2.0;
    f64 epsilon = 0.000001;

    while (TRUE) {
        f64 next_guess = ((exponent - 1) * guess + number / pow(guess, exponent - 1)) / exponent;
        if (fabs(next_guess - guess) < epsilon) {
            return next_guess;
        }
        guess = next_guess;
    }
}

