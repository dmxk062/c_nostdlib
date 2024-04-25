#pragma once
#include "types.h"
#include "memcpy.h"

union Color24Bit {
    u64 value;
    struct {
        u8 r;
        u8 g;
        u8 b;
    };
};


enum AnsiColor {
    DEFAULT = '9',
    BLACK   = '0',
    RED     = '1',
    GREEN   = '2',
    YELLOW  = '3',
    BLUE    = '4',
    MAGENTA = '5',
    CYAN    = '6',
    WHITE   = '7',
    NONE    = '\0',

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

i64 gen_ansi_escape(char* dest, u64 max_len, struct AnsiFormat format);
