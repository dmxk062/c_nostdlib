#include "types.h"
#include <mem.h>


/*
 * TODO: rewrite using SSE
 * slightly faster than a naive implementation that just copies u8s
 * for faster performance, this should be rewritten in asm to take advantage of vector instructions
 */ 
void memcpy(void* dst, const void* src, i64 size) {
    u64* td = (u64*)dst;
    u64* ts = (u64*)src;
    u8 *cd, *cs;

    // copy 8 bytes at once
    for (;size > 8; size -= 8) {
        *td++ = *ts++;
    }

    // we have less than 1 u64 left, copy u8s now
    cd = (u8*)td;
    cs = (u8*)ts;
    
    while(size-- > 0) {
        *cd++ = *cs ++;
    }
}

void* memset(void* dest, u64 val, u64 count) {
    u64* dest64 = (u64*)dest;
    while (count >= 8) {
        *dest64++ = val;
        count -= 8;
    }

    u8* dest8 = (u8*)dest64;
    while (count > 0) {
        *dest8++ = (u8)val;
        count--;
    }

    return dest;
}

bool memeq(const void* buf1, const void* buf2, u64 count) {
    u64* u1 = (u64*) buf1;
    u64* u2 = (u64*) buf2;

    for (; count > 8; count -= 8) {
        if (*u1++ != *u2++) 
            return false;
    }

    u8* b1 = (u8*)u1;
    u8* b2 = (u8*)u2;

    while (count-- > 0) {
        if (*b1++ != *b2++)
            return false;
    }

    return true;
}

u64 count_byte(const u8* buf, u64 len, u8 byte) {
    u64 count = 0;
    for (; len > 0; len--) {
        if (buf[len] == byte) {
            count++;
        }
    }

    return count;
}

Result(u64) find_byte(const u8* buf, u64 len, u8 byte) {
    for (u64 i = 0; i < len; i++) {
        if (buf[i] == byte) 
            return Ok(u64, i);
    }

    return Err(u64, NULL);
}
