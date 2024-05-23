#include "args.h"
#include "types.h"
#include <io.h>
#include <time.h>


i32 main(i32 argc, zstr argv[]) {

    struct Timespec tstamp = {0,0};

    bool show_help = false;
    i64 timezone_offset = 0;
    NamedArguments nargs = {
        {"-h", "--help", &show_help, ArgumentType_BOOL},
        {"-o", "--offset", &timezone_offset, ArgumentType_INT},
    };

    bool got_timestamp = false;
    UnnamedArguments uargs = {
        {&tstamp.secs, ArgumentType_INT, NULL, &got_timestamp},
    };

    u64 num_parsed = parse_arguments(argc-1, argv+1,
            ARRLEN(nargs), nargs,
            ARRLEN(uargs), uargs);

    if (show_help) {
        fwrite(STDERR, "%z: [OPTION]... [TIMESTAMP]\n\n"
            "Options: \n"
            "   -h/--help         : Show this message and exit\n"
            "   -o/--offset HOURS : Add timezone offset\n\n"
            "If TIMESTAMP is not supplied, the current time will be used.\n",
            (fmts){
            {.z = argv[0]}
        });
        return 1;
    }


    if (!got_timestamp) {
        Time_get(&tstamp, NULL);
    }
    tstamp.secs += (60*60) * timezone_offset;

    Time time;
    Time_locatime(&time, &tstamp);
    fprint(
        "%D/%D/%D\n"
        "%D:%D:%D\n"
        , (fmts){
        {.I = {time.years, .padd = 4}},
        {.I = {time.months+1, .padd = 2}},
        {.I = {time.day_in_month, .padd = 2}},

        {.I = {time.hours, .padd = 2}},
        {.I = {time.minutes, .padd = 2}},
        {.I = {time.seconds, .padd = 2}}
    });



    

    return 0;
}
