#include "args.h"


i64 parse_arguments(i64 argc, zstr argv[], u64 named_count, struct named_argument named[], u64 unnamed_count, struct unnamed_argument unnamed[]) {
    i64 recognized_args = 0;
    i64 uindex = 0;
    for (u64 i = 0; i < argc; i++) {
        bool found_arg = FALSE;
        if (argv[i] == NULL) {
            break;
        }
        for (i64 j = 0; j < named_count; j++) {
            if (streq(argv[i], named[j].long_option) || streq(argv[i], named[j].short_option)) {
                bool success = FALSE;
                switch (named[j].type) {
                case STRING: {
                    if (argv[i+1] != NULL) {
                        *(char**)named[j].target = argv[++i];
                        recognized_args+=2;
                        success = TRUE;
                    }
                    break;}
                case BOOL: {
                    *(bool*)named[j].target = TRUE;
                    success = TRUE;
                    recognized_args++;
                    break;}

                case INT: {
                    if (argv[i+1] != NULL) {
                        RESULT(i64) integer = str_to_int(argv[i], strlen(argv[i]), 10);
                        if (integer.success) {
                            *(i64*)named[j].target = integer.value;
                            recognized_args++;
                            success = TRUE;
                        } else {
                            i--;
                        }
                    }
                    recognized_args++;
                    break;}

                case FLOAT: {
                    if (argv[i+1] != NULL) {
                        RESULT(f128) decimal = str_to_float(argv[i], strlen(argv[i]));
                        if (decimal.success) {
                            *(f128*)named[j].target = decimal.value;
                            recognized_args++;
                            success = TRUE;
                        } else {
                            i--;
                        }
                    }
                    recognized_args++;
                    break;}
                }
                found_arg = TRUE;
                if (named[j].found_index != NULL && success)
                    *named[j].found_index = i;
                break;
            }
        }
        if (!found_arg) {
            if (uindex < unnamed_count) {
                bool success = FALSE;
                switch (unnamed[uindex].type) {
                case STRING: {
                    *(char**)unnamed[uindex].target = argv[i];
                    recognized_args++;
                    success = TRUE;
                    break;}

                case INT: {
                    RESULT(i64) integer = str_to_int(argv[i], strlen(argv[i]), 10);
                    if (integer.success) {
                        *(i64*)unnamed[uindex].target = integer.value;
                        recognized_args++;
                        success = TRUE;
                    } 
                    break;}
                case FLOAT: {
                    RESULT(f128) decimal = str_to_float(argv[i], strlen(argv[i]));
                    if (decimal.success) {
                        *(f128*)unnamed[uindex].target = decimal.value;
                        recognized_args++;
                        success = TRUE;
                    }
                    break;}
                }
                if (unnamed[uindex].found_index != NULL && success)
                    *unnamed[uindex].found_index = i;
                uindex++;
            }
        }
    }
    return recognized_args;
}

