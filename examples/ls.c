#include <termcolor.h>
#include <types.h>
#include <io.h>
#include <dirent.h>
#include <args.h>
#include <errno.h>


const static enum AnsiColor FILETYPE_COLORS[] = {
    AnsiColor_WHITE,
    AnsiColor_YELLOW,
    AnsiColor_YELLOW,
    AnsiColor_NONE,
    AnsiColor_CYAN,
    AnsiColor_NONE,
    AnsiColor_MAGENTA,
    AnsiColor_NONE,
    AnsiColor_WHITE,
    AnsiColor_NONE,
    AnsiColor_BLUE,
    AnsiColor_NONE,
    AnsiColor_MAGENTA,
    AnsiColor_NONE,
    AnsiColor_WHITE
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

    bool no_color = false;
    bool no_icons = false;
    bool show_hidden = false;
    NamedArguments nargs = {
        {"-C", "--no-color", &no_color, ArgumentType_BOOL},
        {"-I", "--no-icons", &no_icons, ArgumentType_BOOL},
        {"-a", "--all", &show_hidden,   ArgumentType_BOOL},
    };

    zstr path = ".";
    UnnamedArguments uargs = {
        {&path, ArgumentType_STRING},
    };

    u64 num_parsed = parse_arguments(argc-1, argv+1,
            ARRLEN(nargs), nargs,
            ARRLEN(uargs), uargs);

    Result(u64) dirfd = opendir(path);
    if (!dirfd.ok) {
        fwrite(STDERR, "%eFailed to open %z: %z%E\n", (fmts){
            {.e = {.fg = AnsiColor_RED}},
            {.z = path},
            {.z = ERROR_MESSAGES[dirfd.errno]}
        });
        return dirfd.errno;
    }

    u64 fd = dirfd.value;
    DirectoryBuffer dirbuf;

    PResult(Dirent) ent;
    Dirent* file;

    enum AnsiColor color;
    zstr icon;
    DoWhileSuccessful(ent, nextdir(fd, &dirbuf)) {

        file = ent.value;
        if (!show_hidden && file->name[0] == '.') {
            continue;
        }


        if (no_color)
            color = AnsiColor_NONE;
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
