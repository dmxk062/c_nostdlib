#include "linux/elf.h"
#include "types.h"
#include <linux/vdso.h>
#include <cstring.h>

address VDSO_find_symbol(address vdso_header, zstr name) {
    Elf64_Header* ehdr = (Elf64_Header*)vdso_header;
    Elf64_ProgramHeader* phdr = (Elf64_ProgramHeader*)((u8*)ehdr + ehdr->program_header_offset);

    Elf64_Dynamic* dyn = NULL;

    for (u64 i = 0; i < ehdr->program_header_count; ++i) {
        if (phdr[i].type == Elf64_Tag_DYNAMIC) {
            dyn = (Elf64_Dynamic*)(vdso_header + phdr[i].offset);
            break;
        }
    }

    if (!dyn) return NULL;

    Elf64_Symbol* symbol_table = NULL;
    char* string_table = NULL;

    while (dyn->tag != Elf64_Tag_NULL) {
        if (dyn->tag == Elf64_Tag_SYMBOL_TABLE) {
            symbol_table = (Elf64_Symbol*)(vdso_header + dyn->ptr);
        } else if (dyn->tag == Elf64_Tag_STRING_TABLE) {
            string_table = (char*)(vdso_header + dyn->ptr);
        }
        ++dyn;
    }

    if (!symbol_table||!string_table) return NULL;

    /*
     * The headers on my system say that .other is always 0
     * lets hope it stays that way
     */
    for (;symbol_table->other == 0; ++symbol_table) {
        if (streq(string_table + symbol_table->name, name)) {
            return (address)(vdso_header + symbol_table->value);
        }
    }

    return NULL;
}
