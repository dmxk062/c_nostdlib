#include "args.h"


i64 parse_arguments(i64 argc, zstr argv[], i64 named_count, struct named_argument named[], i64 unnamed_count, struct unnamed_argument unnamed[]) {
    i64 recognized_args = 0;
    i64 uindex = 0;
    for (i64 i = 0; i < argc; i++) {
        bool found_arg = FALSE;
        if (argv[i] == NULL) {
            break;
        }
        for (i64 j = 0; j < named_count; j++) {
            if (streq(argv[i], named[j].long_option) || streq(argv[i], named[j].short_option)) {
                switch (named[j].type) {
                case STRING:
                    if (argv[i+1] != NULL) {
                        *(char**)named[j].target = argv[++i];
                        recognized_args+=2;
                    }
                    break;
                case BOOL:
                    *(bool*)named[j].target = TRUE;
                    recognized_args++;
                    break;

                }
                found_arg = TRUE;
                break;
            }
        }
        if (!found_arg) {
            if (uindex < unnamed_count) {
                switch (unnamed[uindex].type) {
                    case STRING:
                        *(char**)unnamed[uindex].target = argv[i];
                        recognized_args++;
                        uindex++;
                        break;
                }
            }
        }
    }
    return recognized_args;
}

