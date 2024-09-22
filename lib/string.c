#include "io.h"
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
    str->type = StringType_allocated;
    return POk(String, str);
}

errno_t String_free(String* str) {
    if (str->type == StringType_allocated) {
        free(str->buffer);
        free(str);
    } else if (str->type == StringType_view) {
        free(str);
    }

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

PResult(String) String_slice_as_copy(const String* str, i64 start, i64 end) {
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

String* String_slice_as_view(const String* str, i64 start, i64 end) {
    if (start < 0) { start = str->len - start; }
    if (end < 0) { end = str->len - end; }
    if (start > end) {
        return NULL;
    }
    if (end < 0 || start < 0) {
        return NULL;
    }

    if (end > str->len || start > str->len) {
        return NULL;
    }

    String* new = String_new(0).value;
    if (!new) {
        return NULL;
    }

    new->type = StringType_view;
    new->len = end - start;
    new->buffer = str->buffer + start;
    new->size = 0;

    return new;
}


u64 String_split_char_as_copy(String* str, Vec(String)* buffer, char delim) {
    u64 num_split = 0;

    u64 start = 0;
    u64 end   = 0;

    for (i64 i = 0; i < str->len; i++) {
        if (str->buffer[i] == delim) {
            end = i;
            if (!VecpFull(buffer)) {
                PResult(String) new_str = String_slice_as_copy(str, start, end);
                if (!new_str.ok) 
                    return num_split;

                VecpPush(buffer, new_str.value);
                start = end+1;
            } else {
                return num_split;
            }
        }
    }

    if (end < str->len && !(VecpFull(buffer))) {
        PResult(String) new_str = String_slice_as_copy(str, start, str->len);
        if (!new_str.ok) {
            return num_split;
        }
        VecpPush(buffer, new_str.value);
    }

    return num_split;
}

u64 String_split_char_as_view(String* str, Vec(String)* buffer, char delim) {
    u64 num_split = 0;

    u64 start = 0;
    u64 end   = 0;

    for (i64 i = 0; i < str->len; i++) {
        if (str->buffer[i] == delim) {
            end = i;
            if (!VecpFull(buffer)) {
                String* new_str = String_slice_as_view(str, start, end);
                if (!new_str) 
                    return num_split;

                VecpPush(buffer, new_str);
                num_split++;
                start = end+1;
            } else {
                return num_split;
            }
        }
    }

    if (end < str->len && !(VecpFull(buffer))) {
        String* new_str = String_slice_as_view(str, start, str->len);
        if (!new_str) {
            return num_split;
        }
        VecpPush(buffer, new_str);
        num_split++;
    }

    return num_split;
}


PResult(String) StringList_join(Vec(String)* list, String* delim) {
    u64 total_length = 0;
    for (u64 i = 0; i < list->len; i++) {
        total_length += list->vals[i]->len;
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
                list->vals[i]->buffer,
                list->vals[i]->len);
        outindex += list->vals[i]->len;
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

u64 String_count_char(const String* str, char ch) {
    u64 count = 0;
    for (u64 i = 0; i < str->len; i++) {
        if (str->buffer[i] == ch) count++;
    }

    return count;
}
