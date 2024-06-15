#pragma once
#ifndef _MEMCPY_H
#define _MEMCPY_H

#include <types.h>

void memcpy(void* dst, const void* src, i64 size);
void* memset(void* mem, u64 value, u64 count);

bool memeq(const void* buf1, const void* buf2, u64 count);

u64 count_byte(const u8* buf, u64 len, u8 byte);
Result(u64) find_byte(const u8* buf, u64 len, u8 byte);
#endif
