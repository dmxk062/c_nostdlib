#pragma once
#ifndef _STRING_H
#define _STRING_H

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



RESULT(String) String_new(u64 size);
RESULT(String) String_new_from_zstr(zstr char_array);
errno_t String_free(String str);

RESULT(zstr) String_to_zstr(String str);


errno_t String_grow(String str, u64 size);
errno_t String_append(String dst, const String src);

RESULT(String) String_slice(const String str, u64 start, u64 end);

RESULT(StringList) StringList_new(u64 size);
errno_t StringList_free(StringList* strings);

u64 String_split_char(String str, StringList* buffer, char delim);

RESULT(String) StringList_join(StringList* list, String delim);

#endif
