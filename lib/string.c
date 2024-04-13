#include "string.h"
#include "../alloc/alloc.h"
#include "memcpy.h"
#include "va_args.h"


u64 strlen(const char *string) {
    u64 count = 0;
    while (*string++ != '\0') {
        count++;
    }
    return count;
}

u64 strcmp(const char* string1, const char* string2) {
    u64 i = 0;
    while (string1[i] == string2[i]) {
        if (string1[i] == '\0') {
            return 0;
        }
        i++;
    }
    return i;
}


char* strdup(const char* str) {
    u64 len = strlen(str);
    char* buffer = malloc(len + 1);
    if (buffer == NULL) {
        return NULL;
    }
    memcpy(buffer, str, len);
    buffer[len] = '\0';
    return buffer;
}


i64 i_to_base(i64 num, i8 base, char* out, i64 maxlen, u64 padd) {
    static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static const i64 buffsize = 256;

    char buffer[buffsize];
    i64 index = buffsize - 1;
    bool is_negative = FALSE;
    if (num < 0) {
        is_negative = TRUE;
        num = -num;
    }

    while (num != 0) {
        char digit = digits[num % base];
        buffer[--index] = digit;
        num /= base;
    }

    
    i64 length = (buffsize - 1) - index;

    if (padd > length) {
        while (length < padd) {
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
    memcpy(out, addr, length);
    return length;

}

i64 f_to_decimal(f64 num, char* out, i64 maxlen, u64 padd, i64 num_frac) {
    static const char digits[] = "0123456789";
    bool is_negative = FALSE;

    static const i64 buffsize = 128;
    char ibuffer[buffsize];
    char fbuffer[buffsize];

    i64 findex = 0;
    i64 iindex = buffsize;

    if (num < 0) {
        is_negative = TRUE;
        num = -num;
    }

    u64 integer = (u64)num;
    f64 fraction = num - (f64)integer;

    bool had_non0 = FALSE;
    while (integer != 0) {
        char digit = digits[integer % 10];
        if (had_non0 || digit != '0') {
            ibuffer[iindex--] = digit;
            had_non0 = TRUE;
        }
        integer /= 10;
    }
    u64 ilen = (buffsize - 1) - iindex; 

    if (num_frac > 0) {
        for (i64 i = num_frac; i > 0; i--) {
            fraction *= 10;
            u64 digit = (i64)fraction;
            fbuffer[findex++] = '0' + digit;

            fraction -= digit;
        }
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
    i64 total_length = flen + ilen + sizeof(char) + (is_negative * sizeof(char));
    if (padd > total_length) {
        if (padd > maxlen) 
            return -1;
        while (total_length < padd) {
            *out++ = '0';
            total_length++;
        }
    }

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
 *
 * %f : float
 * %F : 128-bit float, rounds to 64bit
 *
 * modifiers:
 *  %_{c,s,d,x,o,b,f} : padd to the left, number of spaces/zeros provided in argv
 *  %.{f,F}           : number of decimals to use, provided in argv
 */
i64 fmt(const char* format, char* out, u64 outlen, ...) {

    u64 fmtlen = strlen(format);
    u64 outind = 0;
    if (outlen < fmtlen) {
        return -1;
    }

    va_list values;
    va_start(values, outlen);

    // loop iterator
    i64 i = 0;
    // padding
    u64 padding = 0;
    u64 decimals = 0;
    u8 base = 0;
    bool was_num = FALSE;
    while (i < fmtlen && outind < outlen) {
        if (format[i] == '%') {
            i++;
            if (format[i] == '_') {
                padding = va_arg(values, u64);
                i++;
            }
            if (format[i] == '.') {
                decimals = va_arg(values, u64);
                i++;
            }
            if (format[i] == '%') {
                out[outind++] = '%';
            } else if (format[i] == 'c') {
                char tmpchar = va_arg(values, int);
                out[outind++] = tmpchar;
            } else if (format[i] == 's') {
                // insert a string
                char* tmpstr = va_arg(values, char*);
                u64 tmplen = strlen(tmpstr);

                // left padding if necessary
                if (padding > 0 && padding > tmplen) {
                    // how much we still need
                    i64 padding_left = padding - tmplen;
                    if (padding + outind > outlen ){
                        return -2;
                    }
                    // pad with spaces
                    while (padding_left > 0) {
                        out[outind++] = ' '; 
                        padding_left--;
                    }
                } else if (tmplen + outind > outlen) {
                    return -2;
                }
                // copy the string into place
                memcpy(out + outind, tmpstr, tmplen);
                outind += tmplen;
            } else if (format[i] == 'f') {
                f64 tmpfloat = va_arg(values, f64);
                i64 length = f_to_decimal(tmpfloat, out + outind, outlen - outind, padding, decimals);
                if (length < 0) {
                    return -3;
                }
                outind += length;
            } else if (format[i] == 'F') {
                f64 tmpfloat = va_arg(values, f128);
                i64 length = f_to_decimal(tmpfloat, out + outind, outlen - outind, padding, decimals);
                if (length < 0) {
                    return -3;
                }
                outind += length;
            } else if (format[i] == 'x') {
                base = 16;
                was_num = TRUE;
            } else if (format[i] == 'd') {
                base = 10;
                was_num = TRUE;
            } else if (format[i] == 'o') {
                base = 8;
                was_num = TRUE;
            } else if (format[i] == 'b') {
                base = 2;
                was_num = TRUE;
            }
            if (was_num) {
                i64 tmpint = va_arg(values, i64);
                i64 length = i_to_base(tmpint, base, out + outind, outlen - outind, padding);
                if (length < 0) {
                    return -2;
                }
                outind += length;

            }
        } else {
            out[outind++] = format[i];
        }
        i++;
        padding = 0;
        decimals = 0;
    }
    if (outind == outlen) {
        return -3;
    }
    out[outind] = '\0';
    va_end(values);
    return outind;
}
