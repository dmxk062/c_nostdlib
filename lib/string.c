#include "include/string.h"
#include "include/memcpy.h"
#include "include/alloc.h"


// fairly simple
u64 strlen(const char *string) {
    u64 count = 0;
    while (*string++ != '\0') {
        count++;
    }
    return count;
}

u64 strcmp(const char* string1, const char* string2) {
    while (*string1 && *string2 && *string1 == *string2) {
        string1++;
        string2++;
    
    }
    return (*string1 - *string2);
}

bool streq(const char* string1, const char* string2) {
    while(*string1 && *string2) {
        if (*string1 != *string2) {
            return FALSE;
        }
        string1++;
        string2++;
    }
    return (*string1 == '\0' && *string2 == '\0');
}

u64 strncmp(const char* string1, const char* string2, u64 len) {
    while (len > 0 && *string1 && *string2 && *string1 == *string2) {
        string1++;
        string2++;
        len--;
    }
    if (len == 0) {
        return 0;
    } else {
        return (*string1 - *string2);
    }
}


/*
 * FIXME: should probably be removed due to implicit allocation
 * just create a buffer yourself and memcpy into it
 */
char* strdup(const char* str) {
    u64 len = strlen(str);
    char* buffer = malloc(len + 1);
    if (buffer == NULL) {
        return NULL;
    }
    memcpy(buffer, str, len);
    buffer[len] = '\0';
    return buffer;
}


i64 strfind(const char* string, u64 len, char c) {
    for (i64 i = 0; i < len; i++) {
        if (string[i] == c)
            return i;
    }
    return -1;
}
