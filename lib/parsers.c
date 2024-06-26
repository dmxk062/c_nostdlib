#include "types.h"
#include <parsers.h>
#include <cstring.h>


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

/*
 * parse a string into an integer, zero error tolerance
 */
Result(i64) str_to_int(const char* string, u64 lenth, u64 base) {
    i64 parsed = 0;

    bool is_negative = false;

    if (string[0] == '-') {
        is_negative = true;
        string++;
        lenth--;
    }

    for (i64 i = 0; i < lenth; i++) {
        i64 digit = char_to_int(string[i]);
        // we got smth we can't really handle, tell the caller at which index
        if (digit == -1 || digit >= base) {
            return Err(i64, i);
        }
        // calculate the positional value
        i64 pwr = 1;
        for (i64 j = 0; j < lenth - 1 -i; j++) {
            pwr *= base;
        }
        parsed += digit * pwr;


    }

    if (is_negative) parsed = -parsed;

    return Ok(i64, parsed);
}

/*
 * zero error tolerance like str_to_int above
 * accepted format:
 * (-)[0-9].[0-9]
 */
Result(f128) str_to_float(const char* string, u64 length) {
    f128 result;

    bool is_negative;
    bool has_decimals;
    u64 target;
    // build up the number of two components
    f128 integer  = 0.0;
    f128 rational = 0.0;

    if (string[0] == '-') {
        is_negative = true;
        string++;
        length--;
    }

    /* 
     * figure out where/if the decimal point is
     * TODO: add support for locale specific ones like ','
    */
    i64 decimal_position = strfind(string, length, '.');
    if (decimal_position < 0) {
        has_decimals = false;
        target = length;
    }  else {
        has_decimals = true;
        target = decimal_position;
    }

    for (i64 i = 0; i < target; i++) {
        i64 digit = char_to_int(string[i]);
        // only base 10 for now
        if (digit == -1 || digit >= 10) {
            return Err(f128, i);
        }

        i64 pwr = 1;
        for (i64 j = 0; j < target - 1 - i; j++) {
            pwr *= 10;
        }
        integer += digit * pwr;
    }

    /*
     * we don't even have to care about the component < 0
     * just return the first part
     */
    if (!has_decimals) {
        if (is_negative) integer = -integer;
        return Ok(f128, integer);
    }

    // iterate over the rest of the string
    for (i64 i = decimal_position + 1; i < length; i++) {
        i64 digit = char_to_int(string[i]);
        if (digit == -1 || digit >= 10) {
            return Err(f128, i);
        }
        // continuously divide, values get smaller as we go right
        f128 pwr = 0.1L;
        for (i64 j = decimal_position + 1; j < i; j++) {
            pwr /= 10.0L;
        }
        rational += digit * pwr;
    }

    result = integer + rational;
    if (is_negative) result = -result;

    return Ok(f128, result);
}
