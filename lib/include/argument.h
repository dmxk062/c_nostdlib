#pragma once
#ifndef _ARGUMENT_H
#define _ARGUMENT_H
#include "types.h"
#include "string.h"

#define ARG_ERROR_LEN 4096


/*
 * Uneven ones require an argument
 */
enum ArgumentType {
    ArgumentType_INT = 1,
    ArgumentType_FLOAT = 3,
    ArgumentType_STRING = 5,
    ArgumentType_CALLBACK = 7,
    ArgumentType_STRINGVEC = 9,

    // without argument
    ArgumentType_TOGGLE = 2,
    ArgumentType_SET = 4,
    ArgumentType_CALLBACK_NO_ARGS = 6,
};

typedef struct {
    void*               dest;
    enum ArgumentType   kind;
    char                short_name;
    zstr                long_name;
    zstr                meta_name;
    zstr                description;
    void*               config;
} ArgDesc_Named;

typedef struct {
    void*               dest;
    enum ArgumentType   kind;
    zstr                meta_name;
    zstr                description;
    bool                required;
    void*               config;
} ArgDesc_Positional;

typedef struct {
    u8                  base;
} ArgConfig_Int;

enum ArgumentResult {
    ArgumentResult_Success = 0,
    ArgumentResult_Error = 1,
    ArgumentResult_Continue = 2,
};

typedef enum ArgumentResult (*ArgumentHandler)(zstr value, void* dest, void* config,  String* error_msg);
typedef enum ArgumentResult (*ArgumentCallback)(zstr value, void* dest, void* data, String* error_msg);

typedef struct {
    ArgumentCallback    callback;
    void*               data;
} ArgConfig_Callback;

errno_t Arguments_parse(u64 argc, zstr argv[argc],
        u64 named_len, ArgDesc_Named[named_len],
        u64 positional_len, ArgDesc_Positional[positional_len],
        zstr program_name, zstr synopsis, zstr description);
#endif
