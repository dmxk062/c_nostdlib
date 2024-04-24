#pragma once
#include "../include/types.h"
#include "string.h"




struct named_argument {
    zstr            long_option;
    zstr            short_option;
    void*           target;
    enum TYPES      type;
};

struct unnamed_argument {
    void*           target;
    enum TYPES      type;
};

i64 parse_arguments(i64 argc, zstr argv[],
        i64 named_count, struct named_argument named[],
        i64 unnamed_count, struct unnamed_argument unnamed[]);

typedef struct named_argument named_args[];
typedef struct unnamed_argument unnamed_args[];
