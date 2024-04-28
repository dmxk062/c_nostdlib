#include <types.h>
#include <memcpy.h>
#include <alloc.h>
#include <cstring.h>
#include <string.h>
#include <errno.h>

RESULT(string) string_new(u64 size) {
    string str = malloc(sizeof(__string_t));
    if (str == NULL)
        return (RESULT(string)){.success = FALSE, .errno = 12};

    str->len = 0;
    if (size == 0) {
        str->buffer = NULL;
        str->size = NULL;
    } else {
        char* buffer = malloc(size * sizeof(char));
        if (buffer == NULL) {
            free(str);
            return (RESULT(string)){.success = FALSE, .errno = 12};
        }
        str->buffer = buffer;
        str->size = size;
    }
    return (RESULT(string)){.success = TRUE, .value = str};
}

RESULT(string) string_new_from_zstr(zstr char_array) {
    u64 length = strlen(char_array);

    RESULT(string) new_string = string_new(length);
    if (!new_string.success) {
        return new_string;
    }
    string str = new_string.value;
    memcpy(str->buffer, char_array, length);
    str->len  = length;

    return (RESULT(string)){.success = TRUE, .value = str};
}


errno_t string_grow(string str, u64 size) {
    if (size <= str->size) {
        return SUCCESS;
    }

    char* buffer = malloc(size);
    if (buffer == NULL) 
        return ENOMEM;

    memcpy(buffer, str->buffer, str->len);

    free(str->buffer);

    str->buffer = buffer;
    return SUCCESS;
}

errno_t string_append(string dst, const string src) {
    u64 new_size = dst->len + src->len;
    if (new_size > dst->size) {
        errno_t ret = string_grow(dst, new_size);
        if (ret > 0) 
            return ret;
    }

    memcpy(dst->buffer + dst->len, src->buffer, src->len);
    dst->len += src->len;
    return SUCCESS;
}

RESULT(string) string_slice(const string str, u64 start, u64 end) {
    u64 new_len = end - start;
    if (end > str->len || new_len > str->len) {
        return (RESULT(string)){.success = FALSE, .errno = 1};
    }

    RESULT(string) new_string = string_new(new_len);
    if (!new_string.success) {
        return new_string;
    }

    string ret = new_string.value;
    ret->len = new_len;
    memcpy(ret->buffer, str->buffer + start, new_len);

    return (RESULT(string)){.success = TRUE, .value = ret};
}

errno_t string_free(string str) {
    free(str->buffer);
    free(str);
    return SUCCESS;
}
