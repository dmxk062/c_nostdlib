#include <math.h>

f128 fabs(f128 x) {
    return x < 0 ? -x : x;
}

u64 abs(i64 x) {
    return x < 0 ? -x : x;
}

f128 pow(f128 base, i64 exponent) {
    f128 result = 1.0;
    i64 i;

    for (i = 0; i < exponent; ++i) {
        result *= base;
    }
    
    return result;
}

f128 root(f128 number, u64 exponent) {
    if (number < 0 && exponent % 2 == 0) {
        // even roots of negative numbers are undefined
        return NAN;
    }

    f128 guess = number / 2.0L;

    // maximum difference
    f128 epsilon = 0.000001L;

    while (true) {
        f128 next_guess = (
                (exponent - 1) * guess 
                + number / pow(guess, exponent - 1)
                ) / exponent;
        if (fabs(next_guess - guess) < epsilon) {
            return next_guess;
        }
        guess = next_guess;
    }
}

