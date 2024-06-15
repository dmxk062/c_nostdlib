#include "structs/map.h"
#include "alloc.h"
#include "types.h"
#include "string.h"
#include "io.h"
#include "mem.h"

#define INPUT_BUFFER_SIZE 1024*8

i32 main(i32 argc, zstr argv[]) {
    Map* map = Map_new();
    if (!map) return 1;
    
    // automatically free a string when replacing or deleting a value
    map->on_destroy = (EntryDestructor)&String_free;

    char input_buffer[INPUT_BUFFER_SIZE];

    Result(u64) num_read;

    DoWhileSuccessful(num_read, read(STDIN, input_buffer, INPUT_BUFFER_SIZE)) {
        if (num_read.value == 0) {
            Map_free(map);
            return Malloc_get_used_count();
        }
        // just the newline
        if (num_read.value == 1) {
            continue;
        }
        // remove final newline
        num_read.value--;

        Result(u64) eq_pos = find_byte((u8*)input_buffer, num_read.value, '=');
        if (!eq_pos.ok) {
            Result(untyped) value = Map_get(map, input_buffer, num_read.value);
            if (value.ok) {
                fprint("%s\n", (fmts){
                    {.s = value.value}
                });
            } else {
                print("not set\n");
            }
            continue;
        }

        if (eq_pos.value + 1 == num_read.value) {
            Map_del(map, input_buffer, num_read.value - 1);
            continue;
        }

        Result(String) new_value = String_new_from_buffer(input_buffer + eq_pos.value + 1, num_read.value - eq_pos.value - 1);
        if (!new_value.ok) {
            print("Failed to alloc memory\n");
            return 1;
        }

        Map_set(map, input_buffer, eq_pos.value, new_value.value);
    }
}
