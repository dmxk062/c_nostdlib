#include <types.h>
#include <mem.h>
#include <alloc.h>
#include <cstring.h>
#include <string.h>
#include <errno.h>
#include <format.h>

PResult(String) String_new(u64 size) {
    String* str = malloc(sizeof(String));
    if (str == NULL)
        return PErr(String, ENOMEM);

    str->len = 0;
    if (size == 0) {
        str->buffer = NULL;
        str->size = NULL;
    } else {
        char* buffer = malloc(size * sizeof(char));
        if (buffer == NULL) {
            free(str);
            return PErr(String, ENOMEM);
        }
        str->buffer = buffer;
        str->size = size;
    }
    str->alloc = true;
    return POk(String, str);
}

errno_t String_free(String* str) {
    if (!str->alloc)
        return SUCCESS;

    free(str->buffer);
    free(str);
    return SUCCESS;
}

PResult(String) String_new_from_buffer(const char* char_array, u64 length) {
    PResult(String) new_string = String_new(length);
    if (!new_string.ok) {
        return new_string;
    }
    String* str = new_string.value;
    memcpy(str->buffer, char_array, length);
    str->len = length;

    return POk(String, str);
}

PResult(String) String_new_from_zstr(zstr char_array) {
    u64 length = strlen(char_array);

    return String_new_from_buffer(char_array, length);
}


Result(zstr) String_to_zstr(String* str) {
    u64 new_length = str->len + sizeof(char);

    zstr new_str = malloc(new_length);
    if (new_str == NULL)
        return Err(zstr, ENOMEM);

    memcpy(new_str, str->buffer, str->len);
    new_str[new_length - sizeof(char)] = '\0';
    return Ok(zstr, new_str);
}


errno_t String_grow(String* str, u64 size) {
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

errno_t String_append_buf(String* dst, zstr char_array, u64 charlen) {
    u64 new_size = dst->len + charlen;
    if (new_size > dst->size) {
        errno_t ret = String_grow(dst, new_size);
        if (ret > 0) 
            return ret;
    }

    memcpy(dst->buffer + dst->len, char_array, charlen);
    dst->len += charlen;
    return SUCCESS;
}
errno_t String_append(String* dst, const String* src) {
    return String_append_buf(dst, src->buffer, src->len);
}

PResult(String) String_slice(const String* str, u64 start, u64 end) {
    u64 new_len = end - start;
    if (new_len == 0) {
        new_len = 1;
    }
    if (end > str->len || new_len > str->len) {
        return PErr(String, 1);
    }

    PResult(String) new_string = String_new(new_len);
    if (!new_string.ok) {
        return new_string;
    }

    String* ret = new_string.value;
    ret->len = new_len;
    memcpy(ret->buffer, str->buffer + start, new_len);

    return POk(String, ret);
}


PResult(StringList) StringList_new(u64 size) {
    StringList* new_list = malloc(sizeof(StringList));
    if (new_list == NULL) 
        return PErr(StringList, ENOMEM);

    new_list->len = 0;
    if (size == 0) {
        new_list->size = 0;
        new_list->strings = NULL;
    } else {
        String** strings = malloc(size * sizeof(String));
        if (strings == NULL) {
            free(new_list);
            return PErr(StringList, ENOMEM);
        }
        new_list->size = size;
        new_list->strings = strings;

    }
    return POk(StringList, new_list);
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

u64 String_split_char(String* str, StringList* buffer, char delim) {
    u64 start_at = buffer->len;
    u64 index = start_at;

    u64 start = 0;
    u64 end   = 0;

    for (i64 i = 0; i < str->len; i++) {
        if (str->buffer[i] == delim) {
            end = i;
            if (index < buffer->size) {
                PResult(String) new_str = String_slice(str, start, end);
                if (!new_str.ok) 
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
        PResult(String) new_str = String_slice(str, start, str->len);
        if (!new_str.ok) {
            return index - start_at;
        }
        buffer->strings[index++] = new_str.value;
        buffer->len++;
    }

    return index - start_at;
}


PResult(String) StringList_join(StringList* list, String* delim) {
    u64 total_length = 0;
    for (u64 i = 0; i < list->len; i++) {
        total_length += list->strings[i]->len;
        if (i != list->len - 1)
            total_length += delim->len;
    }

    PResult(String) new_string = String_new(total_length);
    if (!new_string.ok) 
        return new_string;

    String* str = new_string.value;

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
    return POk(String, str);
}


bool String_buf_eq(const char* char1array, u64 char1len, const char* char2array, u64 char2len) {
    if (char1len != char2len)
        return false;

    u64 size = char1len;
    u64* l1 = (u64*)char1array;
    u64* l2 = (u64*)char2array;

    for (; size > 8; size -= 8) {
        if (*l1++ != *l2++) 
            return false;
    }

    u8* s1 = (u8*)l1;
    u8* s2 = (u8*)l2;

    while (size-- > 0) {
        if (*s1++ != *s2++) 
            return false;
    }

    return true;
}

bool String_eq(const String* str1, const String* str2) {
    return String_buf_eq(str1->buffer, str1->len, str2->buffer, str2->len);
}
