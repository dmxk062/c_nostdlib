#include "memcpy.h"

// slightly faster than naive
void memcpy (void* dst, const void* src, i64 size) {
    u64* td = (u64*)dst;
    u64* ts = (u64*)src;
    char *cd, *cs;

    // copy 8 bytes at once
    for (;size > 8; size -= 8) {
        *td++ = *ts++;
    }

    // we have less than i i64 left, use chars now
    cd = (char*)td;
    cs = (char*)ts;
    
    while(size-- > 0) {
        *cd++ = *cs ++;
    }
}

