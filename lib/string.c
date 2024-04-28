#include <types.h>
#include <memcpy.h>
#include <alloc.h>
#include <cstring.h>
#include <string.h>

RESULT(string) string_new(u64 size) {
    string str = malloc(sizeof(__string_t));
    if (str == NULL)
        return (RESULT(string)){.success = FALSE, .errno = 12};

    if (size == 0) {
        str->len = 0;
        str->buffer = NULL;
        str->size = NULL;
    } else {
        char* buffer = malloc(size * sizeof(char));
        if (buffer == NULL) {
            free(str);
            return (RESULT(string)){.success = FALSE, .errno = 12};
        }
        str->buffer = buffer;
    }
    return (RESULT(string)){.success = TRUE, .value = str};
}

RESULT(string) string_new_from_zstr(zstr char_array) {
    u64 length = strlen(char_array);

    RESULT(string) str = string_new(length);
    if (!str.success) {
        return str;
    }
    string string = str.value;
    memcpy(string->buffer, char_array, length);
    string->size = length;
    string->len  = length;

    return (RESULT(string)){.success = TRUE, .value = string};
}
