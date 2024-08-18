#include "io.h"
#include "linux/elf.h"
#include "process.h"
#include "types.h"
#include <private/vdso.h>
#include <private/libc_start.h>
#include <cstring.h>

struct VDSO vDSO = {
    .initialized = false,
    .vdso_header = NULL,
    .funcs = {
        .time = NULL,
        .getcpu = NULL,
        .clock_gettime = NULL,
        .clock_gettimeofday = NULL,
    }
};


static zstr VDSO_required_symbols[] = {
    "__vdso_clock_gettime",
    "__vdso_clock_gettimeofday",
    "__vdso_time",
    "__vdso_getcpu",
};

void VDSO_init(address vdso_addr) {
    Elf64_Header* ehdr = (Elf64_Header*)vdso_addr;
    Elf64_ProgramHeader* phdr = (Elf64_ProgramHeader*)((u8*)ehdr + ehdr->program_header_offset);
    g_nolibc_global_STATE.vdso.vdso_header = ehdr;

    Elf64_Dynamic* dyn = NULL;
    

    for (u64 i = 0; i < ehdr->program_header_count; i++) {
        if (phdr[i].type == Elf64_Tag_DYNAMIC) {
            dyn = (Elf64_Dynamic*)(vdso_addr + phdr[i].offset);
            break;
        }
    }

    if (!dyn) return;

    Elf64_Symbol* symbol_table = NULL;
    char* string_table = NULL;

    while (dyn->tag != Elf64_Tag_NULL) {
        if (dyn->tag == Elf64_Tag_SYMBOL_TABLE) {
            symbol_table = (Elf64_Symbol*)(vdso_addr + dyn->ptr);
        } else if (dyn->tag == Elf64_Tag_STRING_TABLE) {
            string_table = (char*)(vdso_addr + dyn->ptr);
        }
        ++dyn;
    }

    if (!symbol_table||!string_table) return;

    /*
     * The headers on my system say that .other is always 0
     * lets hope it stays that way
     */
    for (;symbol_table->other == 0; ++symbol_table) {
        zstr symbol_name = string_table + symbol_table->name;
        for (u64 i = 0; i < ARRLEN(VDSO_required_symbols); i++) {
            if (streq(symbol_name, VDSO_required_symbols[i])) {
                g_nolibc_global_STATE.vdso.funcs.funcs[i] =  (address)(vdso_addr + symbol_table->value);
                break;
            }
        }
    }
    if (
            g_nolibc_global_STATE.vdso.funcs.clock_gettime &&
            g_nolibc_global_STATE.vdso.funcs.clock_gettimeofday &&
            g_nolibc_global_STATE.vdso.funcs.time &&
            g_nolibc_global_STATE.vdso.funcs.getcpu 
            ) {
        g_nolibc_global_STATE.vdso.initialized = true;
    }

}
