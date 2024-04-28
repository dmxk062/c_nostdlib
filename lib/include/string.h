#pragma once
#include <types.h>


typedef struct {
    // the available size
    u64     size;
    // index of last used character
    u64     len;
    char*   buffer;
} __string_t;

typedef __string_t* string;
DEFRESULT(string, string);

/*
 * Create a string object from a static c string
 */
#define STRING(cstring) \
    &(__string_t){.size = sizeof(cstring), .len = sizeof(cstring), .buffer = cstring}



RESULT(string) string_new(u64 size);
RESULT(string) string_new_from_zstr(zstr char_array);


errno_t string_grow(string string, u64 size);
errno_t string_append(string dst, const string src);

RESULT(string) string_slice(const string string, u64 start, u64 end);

errno_t string_free(string string);
