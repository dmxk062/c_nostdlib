#include "memcpy.h"
void memcpy (void* dst, const void* src, i64 size) {
    u64* td = (u64*)dst;
    u64* ts = (u64*)src;
    char *cd, *cs;

    for (;size > 8; size -= 8) {
        *td++ = *ts++;
    }

    cd = (char*)td;
    cs = (char*)ts;
    
    while(size-- > 0) {
        *cd++ = *cs ++;
    }
}

