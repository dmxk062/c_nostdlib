#include "string.h"

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

char* u64_to_hex(u64 num) {
    static const char digits[] = "01234567890ABCDEF";
    const u64 num_digits = 16;

    char* out = malloc(num_digits+1);
    if (out == NULL) {
        return NULL;
    }

    for (i64 i = num_digits - 1; i >= 0; i--) {
        out[i] = digits[num & 0xF];
        num >>= 4;
    }
    out[num_digits] = '\0';

    return out;
}


void charcpy(char* dest, const char* source, u64 n) {

    for (u64 i = 0; i < n; i++) {
        dest[i] = source[i];
    }

}

i64 strformat(const char* format, char* output, u64 inlen, char** strings) {
    u64 fmtlen = strlen(format);
    u64 outlen = 0;
    if (fmtlen > inlen) {
        return -1;
    }

    for (u64 i=0; i < fmtlen && outlen < inlen; i++) {
        if (format[i] == '%' && format[i-1] != '\\') {
            u64 len = strlen(*strings);
            if (inlen < len) {
                return -2;
            }
            memcpy(output + outlen, *strings, len);
            outlen += len;
            strings++;
        } else if (format[i] == '\\' && format[i+1] == '%') {
        } else {
            output[outlen] = format[i];
            outlen++;
        }
    }
    output[outlen++] = '\0';
    return --outlen;
    
}
