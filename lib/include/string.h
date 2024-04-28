#pragma once
#include <types.h>


typedef struct {
    // the available size
    u64     size;
    // index of last used character
    u64     len;
    char*   buffer;
} __string_t;

typedef __string_t* String;
DEFRESULT(String, String);

/*
 * Create a string object from a static c string
 */
#define STRING(cstring) \
    &(__string_t){.size = sizeof(cstring), .len = sizeof(cstring), .buffer = cstring}



RESULT(String) string_new(u64 size);
RESULT(String) string_new_from_zstr(zstr char_array);


errno_t string_grow(String string, u64 size);
errno_t string_append(String dst, const String src);

RESULT(String) string_slice(const String string, u64 start, u64 end);

errno_t string_free(String string);
