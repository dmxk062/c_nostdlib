# Assembly

This directory contains the minimal amount of assembly code required to get a C program using `main()` instead of `_start()` to work.

[start.S](start.S): contains the `_start` label that sets up the stack like a C program expects it and then calls `main()`. For this is it places `i32 argc`, `char** argv` and `char** environ` in the appropriate registers so that they're the first, second and third arguments to main respectively(`%rdi`, `%rsi` and `%rdx`). It also saves `environ` to a symbol.

[signal.S](signal.S): contains an ugly workaround for Linux's signal handling. A signal handler needs a `sa_restorer` field that returns from the signal handler. Since this cannot be implemented in C, it's done in assembly here.

[syscall.S](syscall.S): contains simple syscall wrappers
