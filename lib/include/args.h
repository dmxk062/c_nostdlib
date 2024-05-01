#pragma once
#ifndef _ARGS_H
#define _ARGS_H


#include <types.h>




enum ArgumentType {
    ArgumentType_INT,
    ArgumentType_FLOAT,
    ArgumentType_STRING,
    ArgumentType_BOOL,
    ArgumentType_TOGGLE
};

struct NamedArgument {
    zstr            short_option;
    zstr            long_option;
    void*           target;
    enum ArgumentType   type;
    u64*            found_index;
    bool*           success;
};

struct UnnamedArgument {
    void*           target;
    enum ArgumentType   type;
    u64*            found_index;
    bool*           success;
};

u64 parse_arguments(i64 argc, zstr argv[],
        u64 named_count, struct NamedArgument named[],
        u64 unnamed_count, struct UnnamedArgument unnamed[]);

typedef struct NamedArgument NamedArguments[];
typedef struct UnnamedArgument UnnamedArguments[];

#endif 
