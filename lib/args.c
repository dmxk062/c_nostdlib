#include <args.h>
#include <cstring.h>
#include <parsers.h>


u64 parse_arguments(i64 argc, zstr argv[], u64 named_count, struct NamedArgument named[], u64 unnamed_count, struct UnnamedArgument unnamed[]) {
    u64 recognized_args = 0;
    u64 uindex = 0;
    bool parsing_named = TRUE;

    for (u64 i = 0; i < argc; i++) {
        bool found_arg = FALSE;
        if (argv[i] == NULL) {
            break;
        }
        if (parsing_named) {
            if (streq(argv[i], "--")) {
                parsing_named = FALSE;
                found_arg = TRUE;
                continue;
            }
            for (i64 j = 0; j < named_count; j++) {
                if (parsing_named && (streq(argv[i], named[j].long_option) || streq(argv[i], named[j].short_option))) {
                    bool success = FALSE;
                    switch (named[j].type) {
                    case ArgumentType_STRING: {
                        if (argv[i+1] != NULL) {
                            *(char**)named[j].target = argv[+i];
                            recognized_args+=2;
                            success = TRUE;
                        }
                        break;}
                    case ArgumentType_BOOL: {
                        *(bool*)named[j].target = TRUE;
                        success = TRUE;
                        recognized_args++;
                        break;}
                    case ArgumentType_TOGGLE: {
                        *(bool*)named[j].target = !*(bool*)named[j].target;
                        success = TRUE;
                        recognized_args++;
                        break;}

                    case ArgumentType_INT: {
                        if (argv[i+1] != NULL) {
                            RESULT(i64) integer = str_to_int(argv[i+1], strlen(argv[i+1]), 10);
                            if (integer.success) {
                                *(i64*)named[j].target = integer.value;
                                recognized_args++;
                                success = TRUE;
                                i++;
                            } 
                        }
                        recognized_args++;
                        break;}

                    case ArgumentType_FLOAT: {
                        if (argv[i+1] != NULL) {
                            RESULT(f128) decimal = str_to_float(argv[i+1], strlen(argv[i+1]));
                            if (decimal.success) {
                                *(f128*)named[j].target = decimal.value;
                                recognized_args++;
                                success = TRUE;
                                i++;
                            } 
                        }
                        recognized_args++;
                        break;}
                    }
                    found_arg = TRUE;
                    if (named[j].found_index != NULL)
                        *named[j].found_index = i;

                    if (named[j].success != NULL)
                        *named[j].success = success;
                    break;
                }
            }
        }

        if (!parsing_named || !found_arg) {
            if (uindex < unnamed_count) {
                bool success = FALSE;
                switch (unnamed[uindex].type) {
                case ArgumentType_STRING: {
                    *(char**)unnamed[uindex].target = argv[i];
                    success = TRUE;
                    break;}

                case ArgumentType_INT: {
                    RESULT(i64) integer = str_to_int(argv[i], strlen(argv[i]), 10);
                    if (integer.success) {
                        *(i64*)unnamed[uindex].target = integer.value;
                        success = TRUE;
                    } 
                    break;}
                case ArgumentType_FLOAT: {
                    RESULT(f128) decimal = str_to_float(argv[i], strlen(argv[i]));
                    if (decimal.success) {
                        *(f128*)unnamed[uindex].target = decimal.value;
                        success = TRUE;
                    }
                    break;}
                }
                if (unnamed[uindex].found_index != NULL)
                    *unnamed[uindex].found_index = i;

                if (unnamed[uindex].success != NULL)
                    *unnamed[uindex].success = success;
                recognized_args++;
                uindex++;
            }
        }
    }
    return recognized_args;
}

