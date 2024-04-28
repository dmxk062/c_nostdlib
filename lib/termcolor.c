#include <termcolor.h>
#include <syscall.h>
#include <memcpy.h>

i64 ansi_format_escape(char* dest, u64 max_len, struct AnsiFormat format) {
    char buffer[64] = {'\e', '['};
    i64  i = 2;

    if (format.fg != NULL) {
        buffer[i++] = '3';
        buffer[i++] = format.fg;
        buffer[i++] = ';';
    }
    if (format.bg != NULL) {
        buffer[i++] = '4';
        buffer[i++] = format.bg;
        buffer[i++] = ';';
    }
    if (format.bold) {
        buffer[i++] = '1';
        buffer[i++] = ';';
    }
    if (format.italic) {
        buffer[i++] = '3';
        buffer[i++] = ';';
    }
    if (format.underline) {
        buffer[i++] = '4';
        buffer[i++] = ';';
    }
    if (format.strikethrough) {
        buffer[i++] = '9';
        buffer[i++] = ';';
    }

    buffer[--i]   = 'm';
    buffer[++i] = '\0';
    if (i > max_len) {
        return -1;
    }
    memcpy(dest, buffer, i);
    return i;
}
