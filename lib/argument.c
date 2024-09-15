#include "argument.h"
#include "format.h"
#include "io.h"
#include "mem.h"
#include "parsers.h"
#include "string.h"
#include "types.h"
#include <cstring.h>
#include <errno.h>
#include <process.h>


errno_t handler_int(zstr value, void* dest, void* config, String* error_msg) {
    u64 value_len = strlen(value);
    u64 base = 10;
    if (config) {
        base = ((ArgConfig_Int*)config)->base;
    }

    Result(i64) new_int = str_to_int(value, value_len, base);
    if (!new_int.ok) {
        String_format(error_msg, "Invalid base %d integer: %z", Fmt({.i = base}, {.z = value}));
        return 1;
    }

    *(i64*)dest = new_int.value;
    return 0;
}

static errno_t handler_float(zstr value, void* dest, void* config, String* error_msg) {
    u64 value_len = strlen(value);

    Result(f128) new_float = str_to_float(value, value_len);
    if (!new_float.ok) {
        String_format(error_msg, "Invalid floating point number: %z", Fmt({.z = value}));
        return 1;
    }

    *(f64*)dest = new_float.value;
    return 0;
}

static errno_t handler_toggle(zstr value, void* dest, void* config, String* error_msg) {
    *(bool*)dest = !*(bool*)dest;
    return 0;
}
static errno_t handler_set(zstr value, void* dest, void* config, String* error_msg) {
    *(bool*)dest = true;
    return 0;
}

static errno_t handler_string(zstr value, void* dest, void* config, String* error_msg) {
    *(zstr*)dest = value;
    return 0;
}

static errno_t handler_callback(zstr value, void* dest, void* config, String* error_msg) {
    ArgConfig_Callback* callback = config; 
    return callback->callback(value, dest, callback->data, error_msg);
}

static ArgumentHandler builtin_handlers[] = {
    [ArgumentType_INT]      = handler_int,
    [ArgumentType_FLOAT]    = handler_float,
    [ArgumentType_STRING]   = handler_string,
    [ArgumentType_SET]      = handler_set,
    [ArgumentType_TOGGLE]   = handler_toggle,
    [ArgumentType_CALLBACK] = handler_callback,
    [ArgumentType_CALLBACK_NO_ARGS] = handler_callback,
};

#define ABORT_WITH_ERR() \
        fwrite(STDERR, "%z: %s\n", Fmt({.z = program_name}, {.s = error_buffer}));\
        goto abort_parsing;


static void print_help(zstr program_name, zstr synopsis, zstr description,
    u64 named_len, ArgDesc_Named named[named_len],
    u64 positional_len, ArgDesc_Positional positional[positional_len]) 
{
    fprint("Usage: %z", Fmt({.z = program_name}));
    if (named_len > 0) {
        print(" [OPTION]...");
    }
    for (u64 pi = 0; pi < positional_len; pi++) {
        if (positional[pi].required) {
            fprint(" %z", Fmt({.z = positional[pi].meta_name}));
        } else {
            fprint(" [%z]", Fmt({.z = positional[pi].meta_name}));
        }
    }
    fprint("\n%z\n\nOptions:\n", Fmt({.z = synopsis}));

    for (u64 ni = 0; ni < named_len; ni++) {
        ArgDesc_Named cur = named[ni];

        String* long_var = String_new(100).value;
        if (cur.meta_name) {
            String_format(long_var, "--%z=%z", Fmt({.z = cur.long_name}, {.z = cur.meta_name}));
        } else {
            String_format(long_var, "--%z", Fmt({.z = cur.long_name}));
        }
        if (cur.short_name) {
            fprint("  -%c, %S       %z\n", Fmt(
                    {.c = cur.short_name}, {.S = {long_var, .rpadd = 12}}, {.z = cur.description}));
        }
        else {
            fprint("      %S       %z\n", Fmt(
                    {.S = {long_var, .rpadd = 12}}, {.z = cur.description}));
        }
        String_free(long_var);
    }
    print(description);
    
}

errno_t Arguments_parse(u64 argc, zstr argv[argc],
    u64 named_len, ArgDesc_Named named[named_len],
    u64 positional_len, ArgDesc_Positional positional[positional_len],
    zstr program_name, zstr synopsis, zstr description) 
{

    program_name = program_name ?: argv[0];
    if (argc == 1) {
        return 0;
    }      

    PResult(String) new_error_buffer = String_new(ARG_ERROR_LEN);
    if (!new_error_buffer.ok) {
        return ENOMEM;
    }
    String* error_buffer = new_error_buffer.value;

    bool hit_double_dashes = false;

    u64 positional_index = 0;

    for (u64 i = 1; i < argc; i++) {
        if (!hit_double_dashes && streq(argv[i], "--")) {
            hit_double_dashes = true;
            continue;
        }
        zstr arg = argv[i];
        u64 arg_len = strlen(arg);
        errno_t err = 0;


        // some option, not a positional arg, exception: a single '-', as commonly used to indicate a standard stream for I/O
        if(!hit_double_dashes && arg_len > 1 && arg[0] == '-') {
            // a single long option of the form --name[=value]
            if (arg_len > 2 && arg[1] == '-') {
                zstr value = NULL;
                Result(u64) eq_pos = find_byte((u8*)arg, arg_len, '=');
                // no equal sign, value is in next argument, if any
                if (!eq_pos.ok) {
                    value = argv[i+1];
                    arg_len -= 2;
                    arg += 2; // remove initial dashes
                } else {
                    if (eq_pos.value != arg_len-1) {
                        value = arg + eq_pos.value + 1;
                        arg += 2;
                        arg_len = eq_pos.value - 2;
                    } else {
                        arg += 2;
                        arg_len = eq_pos.value - 2;
                    }
                }

                bool found_matching = false;
                for (u64 ni = 0; ni < named_len; ni++) {
                    ArgDesc_Named cur = named[ni];
                    if (!cur.long_name) {
                        continue;
                    }
                    u64 cur_len = strlen(cur.long_name);
                    if (String_buf_eq(cur.long_name, cur_len, arg, arg_len)) {
                        if (value == NULL && cur.kind % 2 != 0) {
                            fwrite(STDERR, "%z: Option '--%z' requires an argument: '%z'\n", Fmt({.z = program_name}, {.z = cur.long_name}, {.z = cur.meta_name}));
                            goto abort_parsing;
                        }

                        // we found smth that is valid, finally
                        err = builtin_handlers[cur.kind](value, cur.dest, cur.config, error_buffer);
                        if (err) {
                            ABORT_WITH_ERR();
                        }
                        if (cur.kind % 2 != 0) { i++; } // skip next arg, used as param
                        found_matching = true;
                        break;
                    }
                }
                if (!found_matching) {
                    if (streq(arg, "help")) {
                        print_help(program_name, synopsis, description, named_len, named, positional_len, positional);
                        exit(1);
                    }
                    fwrite(STDERR, "%z: Unknown long option: --%s\n", Fmt({.z = program_name}, {.s = &(String){.buffer = arg, .alloc = false, .size = arg_len, .len = arg_len}}));
                    goto abort_parsing;
                }
            // a single or multiple short options of the form -xyz
            } else {
                for (u64 ai = 1; ai < arg_len; ai++) {
                    char cur_flag = arg[ai];
                    bool found_matching = false;
                    for (u64 ni = 0; ni < named_len; ni++) {
                        ArgDesc_Named cur = named[ni];
                        if (!cur.short_name) {
                            continue;
                        }

                        if (cur.short_name == cur_flag) {
                            zstr value = argv[i+1];
                            if (cur.kind % 2 != 0 && (ai != arg_len-1 || !value)) {
                                fwrite(STDERR, "%z: Option '-%c' requires an argument: '%z'\n", Fmt({.z = program_name}, {.c = cur_flag}, {.z = cur.meta_name}));
                                goto abort_parsing;
                            }

                            err = builtin_handlers[cur.kind](value, cur.dest, cur.config, error_buffer);
                            if (err) {
                                ABORT_WITH_ERR();
                            }
                            if (cur.kind % 2 != 0) { i++; }
                            found_matching = true;
                            break;
                        }
                    }   
                    if (!found_matching) {
                        if (cur_flag == 'h') {
                            print_help(program_name, synopsis, description, named_len, named, positional_len, positional);
                            exit(1);
                        }
                        fwrite(STDERR, "%z: Unknown short option: -%c\n", Fmt({.z = program_name}, {.c = cur_flag}));
                        goto abort_parsing;
                    }
                }
        }
        // an unnamed argument
        } else {
            if (positional_index < positional_len) {
                ArgDesc_Positional cur = positional[positional_index];
                err = builtin_handlers[cur.kind](arg, cur.dest, cur.config, error_buffer);
                if (err) {
                    ABORT_WITH_ERR();
                    goto abort_parsing;
                }
                positional_index++;
            }
        }
    }

    for (u64 pi = positional_index; pi < positional_len; pi++) {
        if (positional[pi].required) {
            fprint("%z: Missing argument: %z\n", Fmt({.z = program_name}, {.z = positional[pi].meta_name}));
            goto abort_parsing;
        }
    }

    return 0;
abort_parsing:
    fwrite(STDERR, "Try '%z --help' for more information\n", Fmt({.z = program_name}));
    String_free(error_buffer);
    return 1;
}
