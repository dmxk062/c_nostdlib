#pragma once
#include <types.h>




enum argtypes {
    ARGINT,
    ARGFLOAT,
    ARGSTRING,
    ARGBOOL,
    ARGTOGGLE
};

struct NamedArgument {
    zstr            short_option;
    zstr            long_option;
    void*           target;
    enum argtypes   type;
    u64*            found_index;
    bool*           success;
};

struct UnnamedArgument {
    void*           target;
    enum argtypes   type;
    u64*            found_index;
    bool*           success;
};

u64 parse_arguments(i64 argc, zstr argv[],
        u64 named_count, struct NamedArgument named[],
        u64 unnamed_count, struct UnnamedArgument unnamed[]);

typedef struct NamedArgument NamedArguments[];
typedef struct UnnamedArgument UnnamedArguments[];
