#include <termcolor.h>
#include <types.h>
#include <io.h>
#include <dirent.h>
#include <args.h>
#include <errno.h>


const static enum AnsiColor FILETYPE_COLORS[] = {
    CWHITE,
    CYELLOW,
    CYELLOW,
    CNONE,
    CCYAN,
    CNONE,
    CMAGENTA,
    CNONE,
    CWHITE,
    CNONE,
    CBLUE,
    CNONE,
    CMAGENTA,
    CNONE,
    CWHITE
};

const static zstr FILETYPE_ICONS[] = {
    "󰈔 ",
    "󰟥 ",
    " ",
    "",
    "󰉋 ",
    "",
    "󰋊 ",
    "",
    "󰈔 ",
    "",
    "󰌷 ",
    "",
    "󰛳 ",
    "",
    "󰈔 "
};

i32 main(i32 argc, zstr argv[]) {

    bool no_color = FALSE;
    bool no_icons = FALSE;
    bool show_hidden = FALSE;
    NamedArguments nargs = {
        {"-C", "--no-color", &no_color, ARGBOOL},
        {"-I", "--no-icons", &no_icons, ARGBOOL},
        {"-a", "--all", &show_hidden,   ARGBOOL},
    };

    zstr path = ".";
    UnnamedArguments uargs = {
        {&path, ARGSTRING},
    };

    u64 num_parsed = parse_arguments(argc-1, argv+1,
            ARRLEN(nargs), nargs,
            ARRLEN(uargs), uargs);

    RESULT(u64) dirfd = opendir(path);
    if (!dirfd.success) {
        fwrite(STDERR, "%eFailed to open %z: %z%E\n", (fmts){
            {.e = {.fg = CRED}},
            {.z = path},
            {.z = ERROR_MESSAGES[dirfd.errno]}
        });
        return dirfd.errno;
    }

    u64 fd = dirfd.value;
    DirectoryBuffer dirbuf;

    RESULT(dirent) ent;
    Dirent* file;

    enum AnsiColor color;
    zstr icon;
    WHILE_SUCCESS(ent, nextdir(fd, &dirbuf)) {

        file = ent.value;
        if (!show_hidden && file->name[0] == '.') {
            continue;
        }


        if (no_color)
            color = CNONE;
        else 
            color = FILETYPE_COLORS[file->type];

        if (no_icons) 
            icon = "";
        else
            icon = FILETYPE_ICONS[file->type];

        fprint("%e%z%z%E\n", (fmts){
            {.e = {.fg = color}},
            {.z = icon},
            {.z = file->name},
        });
    }
}
