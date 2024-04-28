#pragma once
#include <types.h>


typedef COUNTED_ARRAY(char*, buffer) __string_t;
typedef __string_t* String;

DEFRESULT(String, String);

typedef COUNTED_ARRAY(String*, strings) StringList;
DEFRESULT(StringList*, StringList);


/*
 * Create a string object from a static c string
 */
#define STRING(cstring) \
    &(__string_t){.size = sizeof(cstring), .len = sizeof(cstring), .buffer = cstring}



RESULT(String) string_new(u64 size);
RESULT(String) string_new_from_zstr(zstr char_array);
errno_t string_free(String str);


errno_t string_grow(String str, u64 size);
errno_t string_append(String dst, const String src);

RESULT(String) string_slice(const String str, u64 start, u64 end);

RESULT(StringList) string_list_new(u64 size);
errno_t string_list_free(StringList* strings);

u64 string_split_char(String str, StringList* buffer, char delim);

RESULT(String) string_list_join(StringList* list, String delim);
