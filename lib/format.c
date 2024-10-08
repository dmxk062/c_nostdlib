#include "io.h"
#include "types.h"
#include <mem.h>
#include <cstring.h>
#include <string.h>
#include <format.h>



/*
 * format a number from *base* as a string
 * uses an internal buffer to calculate length and remove leading 0s
 */
i64 i_to_base(i64 num, i8 base, char* out, i64 maxlen, u16 padd) {
    static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const i64 buffsize = 256;

    char buffer[buffsize];
    i64 index = buffsize - 1;
    bool is_negative = false;
    if (num < 0) {
        is_negative = false;
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
    bool is_negative = false;

    // use two separate stack local buffers for integer and fraction 
    const i64 buffsize = 128;
    char ibuffer[buffsize];
    char fbuffer[buffsize];

    i64 findex = 0;
    i64 iindex = buffsize - 1;

    if (num < 0) {
        is_negative = true;
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
 * Does *not* null-terminate the output string
 * returns number of chars written on success or an errno on failure:
 *    - 1: format string too large
 *    - 2: couldnt fit the whole string
 * n > 0: number chars written
 * available formatting sequences:
 * %c : character
 * %z : null terminated string
 * %s : string
 * %d : decimal
 * %x : hex int
 * %o : octal
 * %b : binary
 * %f : float
 * %e : ansi escape sequences
 * %E : reset ansi escape sequences
 * Uppercase versions: with additional format parameters:
 * F: 
 *    - padd: padd with zeroes
 *    - frac: number of decimals
 * {D,X,O,B}:
 *    - padd: padd with zeroes
 * S: 
 *    - rpadd: padd with spaces
 *
 *
 */


Result(u64) fmt(const zstr format, char* out, u64 outlen, fmt_value* values) {
    u64 fmtlen = strlen(format);
    u64 outind = 0;

    // we already couldn't fit the format string into the target, just return
    if (outlen < fmtlen) {
        return Err(u64, 1);
    }

    // loop iterator
    i64 i = 0;

    // for %d, %x, %b, %o
    u8 base = 0;
    bool was_num = false;
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
            } else if (format[i] == 'z' || format[i] == 'Z') {
                char* str_val;
                u16 padding, filling;
                if (format[i] == 'Z') {
                    str_val = values->Z.val;
                    padding = values->Z.lpadd;
                    filling = values->Z.rpadd;
                } else {
                    str_val = values->z;
                    padding = 0;
                    filling = 0;
                }
                values++;
                // insert a string
                u64 len = strlen(str_val);

                // left padding if necessary
                if (padding > 0 && padding > len) {
                    // how much we still need
                    i64 padding_left = padding - len;
                    // check if we have space for that
                    if (padding + outind > outlen ){
                        return Err(u64, 2);
                    }

                    // pad with spaces
                    while (padding_left > 0) {
                        out[outind++] = ' '; 
                        padding_left--;
                    }
                } else if (len + outind > outlen) {
                    return Err(u64, 2);
                }
                // copy the string into place if we have space
                memcpy(out + outind, str_val, len);
                outind += len;
                if (filling > 0 && filling > len) {
                    i64 filling_left = filling - len;
                    if (filling + outind > outlen) {
                        return Err(u64, 2);
                    }

                    while (filling_left > 0) {
                        out[outind++] = ' '; 
                        filling_left--;
                    }
                }

            } else if (format[i] == 's' || format[i] == 'S') {
                String* str_val;
                u16 padding, filling;
                if (format[i] == 'S') {
                    str_val = values->S.val;
                    padding = values->S.lpadd;
                    filling = values->S.rpadd;
                } else {
                    str_val = values->s;
                    padding = 0;
                    filling = 0;
                }
                values++;
                u64 len = str_val->len;
                // left padding if necessary
                if (padding > 0 && padding > len) {
                    // how much we still need
                    i64 padding_left = padding - len;
                    // check if we have space for that
                    if (padding + outind > outlen ){
                        return Err(u64, 2);
                    }

                    // pad with spaces
                    while (padding_left > 0) {
                        out[outind++] = ' '; 
                        padding_left--;
                    }
                } else if (len + outind > outlen) {
                    return Err(u64, 2);
                }
                // copy the string into place if we have space
                memcpy(out + outind, str_val->buffer, len);
                outind += len;
                if (filling > 0 && filling > len) {
                    i64 filling_left = filling - len;
                    if (filling + outind > outlen) {
                        return Err(u64, 2);
                    }
                    while (filling_left > 0) {
                        out[outind++] = ' '; 
                        filling_left--;
                    }
                }

            // ansi format
            } else if (format[i] == 'e') {
                struct AnsiFormat format = values->e;
                values++;
                i64 length = ansi_format_escape(out + outind, outlen - outind, format);
                if (length < 0) {
                    return Err(u64, 2);
                }
                outind += length;
            // reset ansi color escapes
            } else if (format[i] == 'E') {
                if (outlen - outind < sizeof(ANSI_RESET)) {
                    return Err(u64, 2);
                }
                memcpy(out + outind, ANSI_RESET, sizeof(ANSI_RESET));
                outind += sizeof(ANSI_RESET);

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
                    return Err(u64, 2);
                }
                outind += length;

            // all the integer formats, just set the base and then do that in the next step
            } else if (format[i] == 'x' || format[i] == 'X') {
                base = 16;
                was_num = true;
            } else if (format[i] == 'd' || format[i] == 'D') {
                base = 10;
                was_num = true;
            } else if (format[i] == 'o' || format[i] == 'O') {
                base = 8;
                was_num = true;
            } else if (format[i] == 'b' || format[i] == 'B') {
                base = 2;
                was_num = true;
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
                    return Err(u64, 2);
                }
                outind += length;
                was_num = false;

            }
        // otherwise just copy from the format string
        } else {
            out[outind++] = format[i];
        }
        i++;
    }
    return Ok(u64, outind);
}

Result(u64) String_format(String* str, const zstr format, fmt_value* values) {
    Result(u64) ret = fmt(format, str->buffer, str->size, values);
    if (ret.ok) {
        str->len = ret.value;
    }
    return ret;
}
