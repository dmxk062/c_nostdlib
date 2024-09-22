#pragma once
#ifndef _STRING_H
#define _STRING_H

#include <types.h>

enum StringType {
    StringType_allocated,
    StringType_view,
    StringType_readonly,
    StringType_cast,
};

typedef struct {
    u64     size;
    u64     len;
    enum StringType type;
    char*   buffer;
} String;

DefinePResult(String);
DefineVecType(String, String*);

// cast a constant C string as a String
#define STRING(cstring) \
    (String){.size = sizeof(_cstring), .len = sizeof(_cstring) - sizeof(char), .buffer = _cstring, .type = StringType_readonly}

#define STRING_CAST_BUF(_cstring, _length) \
    (String){.buffer = _cstring, .type = StringType_cast, .size = _length, .len = _length}



// allocate a new String
PResult(String) String_new(u64 size);
// allocate a new String based on an existing buffer(with copy)
PResult(String) String_new_from_buffer(const char* char_array, u64 len);
// allocate a new String from a null terminated string(with copy)
PResult(String) String_new_from_zstr(const zstr char_array);
/* Free a String object, does not free stack or statically allocated string objects */
errno_t String_free(String* str);

// create a view on an existing buffer as a string
String* String_view_on_buffer(const char* char_array, u64 len);
// create a view on an existing zstr a string
String* String_view_on_zstr(const zstr char_array);
// create a view on an existing String as a slice
String* String_view_on_slice(const String* str, u64 start, u64 end);

Result(zstr) String_to_zstr(String* str);


bool String_eq(const String* str1, const String* str2);
bool String_buf_eq(const char* char1array, u64 char1len, const char* char2array, u64 char2len);

errno_t String_grow(String* str, u64 size);
errno_t String_append(String* dst, const String* src);

PResult(String) String_slice_as_copy(const String* str, i64 start, i64 end);
String* String_slice_as_view(const String* str, i64 start, i64 end);

u64 String_split_char_as_copy(String* str, Vec(String)* buffer, char delim);
u64 String_split_char_as_view(String* str, Vec(String)* buffer, char delim);

PResult(String) StringList_join(Vec(String)* list, String* delim);

#endif
