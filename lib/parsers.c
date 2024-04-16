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

i64 str_to_int(const char* string, u64 n, u64 base) {
    i64 result = 0;

    bool is_negative = FALSE;

    if (string[0] == '-') {
        is_negative = TRUE;
        string++;
        n--;
    }

    for (i64 i = 0; i < n; i++) {
        i64 digit = char_to_int(string[i]);
        if (digit == -1 || digit >= base) {
            return 0;
        }
        // result += digit * pow(base, n - i - 1);
        i64 pwr = 1;
        for (i64 j = 0; j < n - 1 -i; j++) {
            pwr *= base;
        }
        result += digit * pwr;


    }

    if (is_negative) {
        return -result;
    }
    return result;


}
