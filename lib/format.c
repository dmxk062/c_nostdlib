#include "include/format.h"



/*
 * format a number from *base* as a string
 * uses an internal buffer to calculate length and remove leading 0s
 */
i64 i_to_base(i64 num, i8 base, char* out, i64 maxlen, u16 padd) {
    static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static const i64 buffsize = 256;

    char buffer[buffsize];
    i64 index = buffsize - 1;
    bool is_negative = FALSE;
    if (num < 0) {
        is_negative = TRUE;
        num = -num;
    }

    if (num == 0) {
        buffer[--index] = '0';
    }

    // start from the end so we don't need to reverse it
    while (num != 0) {
        char digit = digits[num % base];
        buffer[--index] = digit;
        num /= base;
    }

    
    i64 length = (buffsize - 1) - index;


    // padd the string if requested
    if (padd > length + (is_negative * sizeof(char))) {
        while (length + (is_negative * sizeof(char)) < padd) {
            buffer[--index] = '0';
            length++;
        }
    }
        
    if (is_negative) {
        buffer[--index] = '-';
        length++;
    }

    if (length > maxlen) {
        return -1;
    }
    char* addr = buffer + index;

    // copy it into the user provided buffer
    memcpy(out, addr, length);
    return length;

}

/*
 * TODO: native support for f128 without rounding down
 * converts a float into a string
 */
i64 f_to_decimal(f128 num, char* out, i64 maxlen, u16 padd, u16 num_frac) {
    static const char digits[] = "0123456789";
    bool is_negative = FALSE;

    // use two separate stack local buffers for integer and fraction 
    static const i64 buffsize = 128;
    char ibuffer[buffsize];
    char fbuffer[buffsize];

    i64 findex = 0;
    i64 iindex = buffsize - 1;

    if (num < 0) {
        is_negative = TRUE;
        num = -num;
    }

    u64 integer = (u64)num;
    f128 fraction = num - (f128)integer;

    if (integer == 0) {
        ibuffer[iindex--] = '0';
    }
    while (integer != 0) {
        char digit = digits[integer % 10];
        ibuffer[iindex--] = digit;
        integer /= 10;
    }
    u64 ilen = (buffsize - 1) - iindex; 

    if (fraction == 0.0) {
        fbuffer[findex++] = '0';
    }
    // if requested by the user, only output num_frac digits after the decimal point
    if (num_frac > 0) {
        for (i64 i = num_frac; i > 0; i--) {
            fraction *= 10;
            u64 digit = (i64)fraction;
            fbuffer[findex++] = '0' + digit;

            fraction -= digit;
        }
    // otherwise exhaust it
    } else {
        while (fraction > 0 && findex < buffsize - 1) {
            fraction *= 10;
            u64 digit = (i64)fraction;
            fbuffer[findex++] = '0' + digit;
            fraction -= digit;
        }
    }
    i64 flen = findex;



    if (is_negative) {
        *out++ = '-';
    }

    // calculate and do padding
    i64 total_length = flen + ilen + sizeof(char) + (is_negative * sizeof(char));
    if (padd > total_length) {
        if (padd > maxlen) 
            return -1;
        while (total_length < padd) {
            *out++ = '0';
            total_length++;
        }
    }

    // construct the final string
    memcpy(out, ibuffer + iindex + 1, ilen);
    out[ilen] = '.';
    memcpy(out + 1 + ilen, fbuffer, findex);
    return total_length;


}

/*
 * Return codes:
 * -1: output string too small for format string
 * -2: values too large
 * -3: failed to null-terminate, use safely with length
 * n > 0: number chars written
 * available formatting sequences:
 * %c : character
 * %s : string
 * %d : decimal
 * %x : hex int
 * %o : octal
 * %b : binary
 * %f : float
 * %a : ansi escape sequences
 * Uppercase versions: with additional format parameters:
 * F: 
 *    - padd: padd with zeroes
 *    - frac: number of decimals
 * {D,X,O,B}:
 *    - padd: padd with zeroes
 * S: 
 *    - padd: padd with spaces
 *
 *
 */
i64 fmt(const char* format, char* out, u64 outlen, fmt_value* values) {
    u64 fmtlen = strlen(format);
    u64 outind = 0;

    // we already couldn't fit the format string into the target, just return
    if (outlen < fmtlen) {
        return -1;
    }

    // loop iterator
    i64 i = 0;

    // for %d, %x, %b, %o
    u8 base = 0;
    bool was_num = FALSE;
    while (i < fmtlen && outind < outlen) {
        if (format[i] == '%') {
            i++;
            // treat double % as an escaped version
            if (format[i] == '%') {
                out[outind++] = '%';

            // single character
            } else if (format[i] == 'c') {
                char tmpchar = values->c;
                values++;
                out[outind++] = tmpchar;
            } else if (format[i] == 's' || format[i] == 'S') {
                char* str_val;
                u16 padding;
                if (format[i] == 'S') {
                    str_val = values->S.val;
                    padding = values->S.padd;
                    values++;
                } else {
                    str_val = values->s;
                    values++;
                    padding = 0;
                }
                // insert a string
                u64 len = strlen(str_val);

                // left padding if necessary
                if (padding > 0 && padding > len) {
                    // how much we still need
                    i64 padding_left = padding - len;
                    // check if we have space for that
                    if (padding + outind > outlen ){
                        return -2;
                    }

                    // pad with spaces
                    while (padding_left > 0) {
                        out[outind++] = ' '; 
                        padding_left--;
                    }
                } else if (len + outind > outlen) {
                    return -2;
                }
                // copy the string into place if we have space
                memcpy(out + outind, str_val, len);
                outind += len;

            } else if (format[i] == 'a') {
                struct AnsiFormat format = values->a;
                values++;
                i64 length = ansi_format_escape(out + outind, outlen - outind, format);
                if (length < 0) {
                    return -2;
                }
                outind += length;

            // floats
            } else if (format[i] == 'f' || format[i] == 'F') {
                f128 float_val;
                u16 padding;
                u16 decimals;
                if (format[i] == 'F') {
                    float_val = values->F.val;
                    padding = values->F.padd;
                    decimals = values->F.frac;
                    values++;
                } else {
                    float_val = values->f;
                    padding = decimals = 0;
                    values++;
                }
                i64 length = f_to_decimal(float_val, out + outind, outlen - outind, padding, decimals);
                if (length < 0) {
                    return -2;
                }
                outind += length;

            // all the integer formats, just set the base and then do that in the next step
            } else if (format[i] == 'x' || format[i] == 'X') {
                base = 16;
                was_num = TRUE;
            } else if (format[i] == 'd' || format[i] == 'D') {
                base = 10;
                was_num = TRUE;
            } else if (format[i] == 'o' || format[i] == 'O') {
                base = 8;
                was_num = TRUE;
            } else if (format[i] == 'b' || format[i] == 'B') {
                base = 2;
                was_num = TRUE;
            }
            // actually format the number
            if (was_num) {
                i64 int_val;
                u16 padding;
                if (format[i] <= 'Z') {
                    int_val = values->I.val;
                    padding = values->I.padd;
                    values++;
                } else {
                    int_val = values->i;
                    values++;
                    padding = 0;
                }
                i64 length = i_to_base(int_val, base, out + outind, outlen - outind, padding);
                if (length < 0) {
                    return -2;
                }
                outind += length;
                was_num = FALSE;

            }
        // otherwise just copy from the format string
        } else {
            out[outind++] = format[i];
        }
        i++;
    }
    // return -3 if we succeeded but just failed to null terminate, caller can still use this
    if (outind == outlen) {
        return -3;
    }
    out[outind] = '\0';
    return outind;
}


