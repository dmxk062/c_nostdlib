#include <format.h>
#include <types.h>
#include <constants.h>
#include <errno.h>
#include <alloc.h>
#include <string.h>
#include <filesystem.h>
#include <cstring.h>
#include <utils/path.h>


PathList* PathList_new_from_zstr(zstr path, char sep) {
    u64 length = strlen(path);
    String str = STRING_CAST_BUF(path, length);

    u64 num_separators = String_count_char(&str, sep);
    Vec(String)* vec = malloc(sizeof(Vec(String)));
    String** buf = malloc(sizeof(String*) * (num_separators + 1));
    if (!vec || !buf) {
        return NULL;
    }

    *vec = (Vec(String)){.len = 0, .size = num_separators + 1, .vals = buf};

    u64 num_elems = String_split_char_as_view(&str, vec, sep);
    if (num_elems == 0) {
        return NULL;
    }
    vec->len = num_elems;

    PathList* list = malloc(sizeof(PathList));
    if (!list) {
        return NULL;
    }

    *list = (PathList){.path = vec};
    return list;
}

PResult(String) PathList_lookup_name(PathList* list, String* name, enum AccessMode mode) {
    PResult(String) new_buffer = String_new(PATH_MAX);
    if (!new_buffer.ok) {
        return PErr(String, ENOMEM);
    }
    String* buffer = new_buffer.value;
    
    for (u64 i = 0; i < list->path->len; i++) {
        String* path= list->path->vals[i];
        Result(u64) ret = String_format(buffer, "%s/%s", Fmt({.s = path}, {.s = name}));
        if (!ret.ok) {
            String_free(buffer);
            return PErr(String, ret.errno);
        } else if (ret.value >= PATH_MAX) {
            String_free(buffer);
            return PErr(String, ENAMETOOLONG);
        }

        // HACK:
        // so we can pass it directly to linux without having to turn it into a zstr
        buffer->buffer[buffer->len] = '\0';

        if (access(buffer->buffer, mode) == 0) {
            return POk(String, buffer);
        }
    }

    return PErr(String, 0);
}

void PathList_free(PathList* list) {
    Vec(String)* path = list->path;
    String* to_free;
    VecpForeach(path, to_free) {
        String_free(to_free);
    }

    free(list);
}
