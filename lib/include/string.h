#pragma once
#ifndef _STRING_H
#define _STRING_H

#include <types.h>


typedef struct {
    u64     size;
    u64     len;
    bool    alloc;
    char*   buffer;
} String;

DefinePResult(String);

typedef COUNTED_ARRAY(String*, strings) StringList;
DefinePResult(StringList);


/*
 * Create a string object from a static c string, mark it as static so it wont be freed
 */
#define STRING(cstring) \
    (String){.size = sizeof(cstring), .len = sizeof(cstring) - sizeof(char), .buffer = cstring, .alloc = false}



PResult(String) String_new(u64 size);
PResult(String) String_new_from_buffer(const char* char_array, u64 len);
PResult(String) String_new_from_zstr(const zstr char_array);
/* Free a String object, does not free stack or statically allocated string objects */
errno_t String_free(String* str);

Result(zstr) String_to_zstr(String* str);


bool String_eq(const String* str1, const String* str2);
bool String_buf_eq(const char* char1array, u64 char1len, const char* char2array, u64 char2len);

errno_t String_grow(String* str, u64 size);
errno_t String_append(String* dst, const String* src);

PResult(String) String_slice(const String* str, u64 start, u64 end);

PResult(StringList) StringList_new(u64 size);
errno_t StringList_free(StringList* strings);

u64 String_split_char(String* str, StringList* buffer, char delim);

PResult(String) StringList_join(StringList* list, String* delim);

#endif
