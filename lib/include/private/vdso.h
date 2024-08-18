#pragma once
#ifndef _LINUX_VDSO_H
#define _LINUX_VDSO_H

#include "linux/elf.h"
#include <types.h>
#include "time.h"

// There should be **no** need to use it manually
// address VDSO_find_symbol(address vdso_header, zstr name);

union VDSO_functions {
    struct {
        i32 (*clock_gettime)(enum Clocks, struct Timespec* dest);
        i32 (*clock_gettimeofday)(struct Timespec* dest, struct Timezone* zone);
        i32 (*time)(u64* dest);
        i32 (*getcpu)(u32* cpu, u32* node);
    };
    address funcs[4];
};

struct VDSO {
    bool initialized;
    Elf64_Header* vdso_header;
    union VDSO_functions funcs;
};

void VDSO_init(address vdso_addr);

#endif
