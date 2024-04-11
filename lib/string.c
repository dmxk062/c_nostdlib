#include "string.h"

u64 strlen(const char *string) {
    u64 count = 0;
    while (*string++ != '\0') {
        count++;
    }
    return count;
}

u64 strcmp(const char* string1, const char* string2) {
    u64 i = 0;
    while (string1[i] == string2[i]) {
        if (string1[i] == '\0') {
            return 0;
        }
        i++;
    }
    return i;
}
