#include <filesystem.h>
#include <format.h>
#include <process.h>
#include <stat.h>
#include <string.h>
#include <utils/path.h>
#include <argument.h>
#include <io.h>
#include <mem.h>
#include <cstring.h>
#include <process.h>
#include <types.h>
#include <private/environ.h>
#include <private/libc_start.h>


const zstr ProgramName = "env";
const zstr Synopsis = "Execute program with custom environment";
const zstr Description = "\n"
                         "Set values in environment for key=value pairs\n"
                         "After last key=value pair is encountered, WORDs are treated as a program to execute\n"
                         "If the program is not an absolute or relative path, $PATH is searched for it\n";

bool delete_environment = false;
ArgDesc_Named named[] = {
    {&delete_environment, ArgumentType_SET, 'c', "clear-environment", NULL, "Start with empty environment"},
};
Vec(zstr) arguments = VecNew(zstr, 1024);
ArgDesc_Positional positional[] = {
    {&arguments, ArgumentType_STRINGVEC, "WORDS", "key=value pairs and program to execute", false},
};

i32 main(i32 argc, zstr argv[], zstr envv[]) {
    zstr path = getenv("PATH").value;
    errno_t err = Arguments_parse(argc, argv,
            ARRLEN(named), named,
            ARRLEN(positional), positional,
            ProgramName, Synopsis, Description);

    if (err) { return err; }

    struct Environment* environ;
    if (delete_environment) {
        environ = Environment_new(ENV_PAGE_SIZE);
        if (!environ) {
            return 1;
        }
    } else {
        environ = g_nolibc_global_STATE.environ;
    }

    u64 argindex = 0;
    for (; argindex < arguments.len; argindex++) {
        zstr cur_arg = VecGet(arguments, argindex);
        u64 cur_len = strlen(cur_arg);
        Result(u64) eq_pos = find_byte((u8*)cur_arg, cur_len, '=');

        // no = sign anymore, start processing as program
        if (!eq_pos.ok) {
            break;
        }
        u64 name_len = eq_pos.value;
        char* value = cur_arg + name_len + 1;
        u64 value_len = cur_len - eq_pos.value;
        if (value_len == 0) {
            Environment_unset(environ, cur_arg, name_len);
        } else {
            Environment_set(environ, cur_arg, name_len, value, value_len, true);
        }
    }

    // nothing left to do, print environment
    if (argindex == arguments.len) {
        for (u64 i = 0; i < environ->env_count; i++) {
            print(environ->env[i]);
            print("\n");
        }
        return 0;
    }

    // get the program to execute from $PATH if necessary
    zstr program = VecGet(arguments, argindex);
    if (stat(program, NULL) != 0) {
        PathList* exec_path = PathList_new_from_zstr(path, ':');
        u64 program_len = strlen(program);
        PResult(String) program_path = PathList_lookup_name(exec_path, &STRING_CAST_BUF(program, program_len), AccessMode_F | AccessMode_X);

        if (!program_path.ok) {
            fwrite(STDERR, "env: '%z' No such file or directory\n", Fmt({.z = program}));
            PathList_free(exec_path);
            return 1;
        }

        Result(zstr) new_program = String_to_zstr(program_path.value);
        program = new_program.value;
    }

    Process_exec(program, &arguments.vals[argindex], environ->env);
}
