#include "memcpy.h"


/*
 * TODO: rewrite using SSE
 * slightly faster than a naive implementation that just copies chars
 * for faster performance, this should be rewritten in asm to take advantage of vector instructions
 */ 
void memcpy (void* dst, const void* src, i64 size) {
    u64* td = (u64*)dst;
    u64* ts = (u64*)src;
    char *cd, *cs;

    // copy 8 bytes at once
    for (;size > 8; size -= 8) {
        *td++ = *ts++;
    }

    // we have less than 1 i64 left, copy chars now
    cd = (char*)td;
    cs = (char*)ts;
    
    while(size-- > 0) {
        *cd++ = *cs ++;
    }
}

