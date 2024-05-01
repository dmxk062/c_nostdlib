#pragma once
#ifndef _MEMCPY_H
#define _MEMCPY_H

#include <types.h>

void memcpy(void* dst, const void* src, i64 size);
void* memset(void* mem, u64 value, u64 count);
#endif
