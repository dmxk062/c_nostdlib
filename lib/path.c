#include "format.h"
#include "types.h"
#include <string.h>
#include <filesystem.h>
#include "utils/path.h"

errno_t find_in_path(struct PathAccess* path, String out, String search) {
    StringList* PathDirs = path->path;
    RESULT(u64) num_written;

    for (u64 i = 0; i < PathDirs->len; i++) {

        num_written = String_format("%s/%s ", out, (fmts){{.s = PathDirs->strings[i]}, {.s = search}});
        if (!num_written.success)
            return num_written.errno;
        out->buffer[num_written.value-1] = '\0';
        if (access(out->buffer, AccessMode_F | AccessMode_X) == 0) {
            out->len--;
            return 0;
        }
    }

    out->len = 0;
    return 1;
}
