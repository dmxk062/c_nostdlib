#include <types.h>
#include <memcpy.h>
#include <alloc.h>
#include <cstring.h>
#include <string.h>
#include <errno.h>
#include <format.h>

RESULT(String) String_new(u64 size) {
    String str = malloc(sizeof(__string_t));
    if (str == NULL)
        return (RESULT(String)){.success = FALSE, .errno = ENOMEM};

    str->len = 0;
    if (size == 0) {
        str->buffer = NULL;
        str->size = NULL;
    } else {
        char* buffer = malloc(size * sizeof(char));
        if (buffer == NULL) {
            free(str);
            return (RESULT(String)){.success = FALSE, .errno = ENOMEM};
        }
        str->buffer = buffer;
        str->size = size;
    }
    return (RESULT(String)){.success = TRUE, .value = str};
}

errno_t String_free(String str) {
    free(str->buffer);
    free(str);
    return SUCCESS;
}

RESULT(String) String_new_from_zstr(zstr char_array) {
    u64 length = strlen(char_array);

    RESULT(String) new_string = String_new(length);
    if (!new_string.success) {
        return new_string;
    }
    String str = new_string.value;
    memcpy(str->buffer, char_array, length);
    str->len  = length;

    return (RESULT(String)){.success = TRUE, .value = str};
}

RESULT(zstr) String_to_zstr(String str) {
    u64 new_length = str->len + sizeof(char);

    zstr new_str = malloc(new_length);
    if (new_str == NULL)
        return (RESULT(zstr)){.success = FALSE, .errno = ENOMEM};

    memcpy(new_str, str->buffer, str->len);
    new_str[new_length] = '\0';
    return (RESULT(zstr)){.success = TRUE, .value = new_str};
}


errno_t String_grow(String str, u64 size) {
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

errno_t String_append(String dst, const String src) {
    u64 new_size = dst->len + src->len;
    if (new_size > dst->size) {
        errno_t ret = String_grow(dst, new_size);
        if (ret > 0) 
            return ret;
    }

    memcpy(dst->buffer + dst->len, src->buffer, src->len);
    dst->len += src->len;
    return SUCCESS;
}

RESULT(String) String_slice(const String str, u64 start, u64 end) {
    u64 new_len = end - start;
    if (new_len == 0) {
        new_len = 1;
    }
    if (end > str->len || new_len > str->len) {
        return (RESULT(String)){.success = FALSE, .errno = 1};
    }

    RESULT(String) new_string = String_new(new_len);
    if (!new_string.success) {
        return new_string;
    }

    String ret = new_string.value;
    ret->len = new_len;
    memcpy(ret->buffer, str->buffer + start, new_len);

    return (RESULT(String)){.success = TRUE, .value = ret};
}


RESULT(StringList) StringList_new(u64 size) {
    StringList* new_list = malloc(sizeof(StringList));
    if (new_list == NULL) 
        return (RESULT(StringList)){.success = FALSE, .errno = ENOMEM};

    new_list->len = 0;
    if (size == 0) {
        new_list->size = 0;
        new_list->strings = NULL;
    } else {
        String* strings = malloc(size * sizeof(__string_t));
        if (strings == NULL) {
            free(new_list);
            return (RESULT(StringList)){.success = FALSE, .errno = ENOMEM};
        }
        new_list->size = size;
        new_list->strings = strings;

    }
    return (RESULT(StringList)){.success = TRUE, .value = new_list};
}

errno_t StringList_free(StringList* strings) {
    for (u64 i = 0; i < strings->len; i++) {
        if (strings->strings[i] != NULL)
            String_free(strings->strings[i]);
    }
    free(strings->strings);
    free(strings);
    return SUCCESS;
}

u64 String_split_char(String str, StringList* buffer, char delim) {
    u64 start_at = buffer->len;
    u64 index = start_at;

    u64 start = 0;
    u64 end   = 0;

    for (i64 i = 0; i < str->len; i++) {
        if (str->buffer[i] == delim) {
            end = i;
            if (index < buffer->size) {
                RESULT(String) new_str = String_slice(str, start, end);
                if (!new_str.success) 
                    return index - start_at;

                buffer->strings[index++] = new_str.value;
                buffer->len++;
                start = end+1;
            } else {
                return index - start_at;
            }
        }
    }

    if (end < str->len && index < buffer->size) {
        RESULT(String) new_str = String_slice(str, start, str->len);
        if (!new_str.success) {
            return index - start_at;
        }
        buffer->strings[index++] = new_str.value;
        buffer->len++;
    }

    return index - start_at;
}


RESULT(String) StringList_join(StringList* list, String delim) {
    u64 total_length = 0;
    for (u64 i = 0; i < list->len; i++) {
        total_length += list->strings[i]->len;
        if (i != list->len - 1)
            total_length += delim->len;
    }

    RESULT(String) new_string = String_new(total_length);
    if (!new_string.success) 
        return new_string;

    String str = new_string.value;

    u64 outindex = 0;

    for(u64 i = 0; i < list->len; i++) {
        memcpy(str->buffer + outindex,
                list->strings[i]->buffer,
                list->strings[i]->len);
        outindex += list->strings[i]->len;
        if (i != list->len - 1) {
            memcpy(str->buffer + outindex,
                    delim->buffer,
                    delim->len);
            outindex += delim->len;
        }
    }

    str->len=total_length;
    return (RESULT(String)){.success = TRUE, .value = str};
}

