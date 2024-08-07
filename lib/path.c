#include "format.h"
#include "types.h"
#include <string.h>
#include <filesystem.h>
#include "utils/path.h"

errno_t find_in_path(struct PathAccess* path, String* out, String* search, u64 mode) {
    StringList* PathDirs = path->path;
    Result(u64) num_written;

    for (u64 i = 0; i < PathDirs->len; i++) {

        num_written = String_format(out, "%s/%s ", (fmts){{.s = PathDirs->strings[i]}, {.s = search}});
        if (!num_written.ok)
            return num_written.errno;
        out->buffer[num_written.value-1] = '\0';
        if (access(out->buffer, mode) == 0) {
            out->len--;
            return 0;
        }
    }

    out->len = 0;
    return 1;
}
