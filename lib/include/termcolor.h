#pragma once
#include "types.h"

enum AnsiColor {
    CDEFAULT = '9',
    CBLACK   = '0',
    CRED     = '1',
    CGREEN   = '2',
    CYELLOW  = '3',
    CBLUE    = '4',
    CMAGENTA = '5',
    CCYAN    = '6',
    CWHITE   = '7',
    CNONE    = '\0',

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
