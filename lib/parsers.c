#include "parsers.h"


inline i64 char_to_int(char c) {
    if ( c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'z') {
        return c - 'a' + 10;
    } else if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 10;
    }
    return -1;
}

RESULT(i64) str_to_int(const char* string, u64 n, u64 base) {
    i64 parsed = 0;

    bool is_negative = FALSE;

    if (string[0] == '-') {
        is_negative = TRUE;
        string++;
        n--;
    }

    for (i64 i = 0; i < n; i++) {
        i64 digit = char_to_int(string[i]);
        if (digit == -1 || digit >= base) {
            return (RESULT(i64)){.success = FALSE, .errno = i};
        }
        // result += digit * pow(base, n - i - 1);
        i64 pwr = 1;
        for (i64 j = 0; j < n - 1 -i; j++) {
            pwr *= base;
        }
        parsed += digit * pwr;


    }

    if (is_negative) {
        parsed = -parsed;
    }
    return (RESULT(i64)){.success = TRUE, .value = parsed};
}

RESULT(f128) str_to_float(const char* string, u64 n) {
    f64 result;

    bool is_negative;
    bool has_decimals;
    u64 target;
    f128 integer = 0;
    f128 rational = 0.0;

    i64 i = 0;
    if (string[0] == '-') {
        is_negative = TRUE;
        string++;
        n--;
    }

    i64 decimal_position = strfind(string, n, '.');
    if (decimal_position < 0) {
        has_decimals = FALSE;
        target = n;
    }  else {
        has_decimals = TRUE;
        target = decimal_position;
    }

    for (i64 i = 0; i < target; i++) {
        i64 digit = char_to_int(string[i]);
        if (digit == -1 || digit >= 10) {
            return (RESULT(f128)){.success = FALSE, .errno = i};
        }

        i64 pwr = 1;
        for (i64 j = 0; j < target - 1 - i; j++) {
            pwr *= 10;
        }
        integer += digit * pwr;
    }

    if (!has_decimals) {
        if (is_negative) integer = -integer;
        result = (f64)integer;
        return (RESULT(f128)){.success = TRUE, .value = result};
    }

    for (i64 i = decimal_position + 1; i < n; i++) {
        i64 digit = char_to_int(string[i]);
        if (digit == -1 || digit >= 10) {
            return (RESULT(f128)){.success = FALSE, .errno = i};
        }
        f128 pwr = 0.1L;
        for (i64 j = decimal_position + 1; j < i; j++) {
            pwr /= 10.0L;
        }
        rational += digit * pwr;
    }

    result = integer + rational;
    if (is_negative) result = -result;

    return (RESULT(f128)){.success = TRUE, .value = result};
}
