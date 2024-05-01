#pragma once
#include <types.h>

enum AnsiColor {
    AnsiColor_DEFAULT = '9',
    AnsiColor_BLACK   = '0',
    AnsiColor_RED     = '1',
    AnsiColor_GREEN   = '2',
    AnsiColor_YELLOW  = '3',
    AnsiColor_BLUE    = '4',
    AnsiColor_MAGENTA = '5',
    AnsiColor_CYAN    = '6',
    AnsiColor_WHITE   = '7',
    AnsiColor_NONE    = '\0',

};

struct AnsiFormat {
    enum AnsiColor fg;
    enum AnsiColor bg;
    bool bold;
    bool italic;
    bool underline;
    bool strikethrough;
};


#define ANSI_RESET "\e[0m"

i64 ansi_format_escape(char* dest, u64 max_len, struct AnsiFormat format);
