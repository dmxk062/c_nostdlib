#include "string.h"

u64 strlen(const char *string) {
    u64 count = 0;
    while (*string != '\0') {
        count++;
        string++;
    }
    return count;
}
