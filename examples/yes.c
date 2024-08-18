#include "alloc.h"
#include "signal.h"
#include "io.h"
#include "string.h"
#include "types.h"

volatile bool received_interrupt = false;


void handler(i64 signum) {
    received_interrupt = true;
    return;
}

i32 main(i32 argc, zstr argv[]) {
    String* output;

    if (argc == 1) {
        output = &STRING("y\n");
    } else {
        PResult(StringList) arguments = StringList_new(argc - 1);
        if (!arguments.ok) {
            return 1;
        }
        StringList* argument_list = arguments.value;
        for (u64 i = 1; i < argc; i++) {
            PResult(String) str = String_new_from_zstr(argv[i]);
            if (!str.ok) {
                return 1;
            }
            argument_list->strings[argument_list->len++] = str.value;
        }
        PResult(String) new_joined = StringList_join(argument_list, &STRING(" "));
        if (!new_joined.ok) {
            return 1;
        }
        output = new_joined.value;
        StringList_free(argument_list);
        String_append(output, &STRING("\n"));
    }

    for (u64 sig = 0; sig < Signal_TERM; sig++) {
        Signal_action(sig, handler, 0, 0, NULL);
    }
    while (!received_interrupt) {
        write(STDOUT, output->buffer, output->len);
    }

    String_free(output);
    return Alloc_get_in_use();
}
