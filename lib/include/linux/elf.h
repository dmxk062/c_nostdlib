#pragma once
#ifndef _LINUX_ELF_H
#define _LINUX_ELF_H

#include <types.h>

enum Elf64_Tags {
    Elf64_Tag_NULL = 0,
    Elf64_Tag_LOAD = 1,
    Elf64_Tag_DYNAMIC = 2,
    Elf64_Tag_STRING_TABLE = 5,
    Elf64_Tag_SYMBOL_TABLE = 6,
};

typedef struct {
    u8      magic_number[16];
    u16     type;
    u16     machine;
    u32     version;
    u64     entry;
    u64     program_header_offset;
    u64     section_header_offset;
    u32     flags;
    u16     header_size;
    u16     program_header_size;
    u16     program_header_count;
    u16     section_header_size;
    u16     section_header_count;
    u16     section_header_strindex;
} Elf64_Header;

typedef struct {
    u32     name;
    u32     type;
    u64     flags;
    u64     offset;
    u64     size;
    u32     link;
    u32     info;
    u64     alignment;
    u64     entry_size;
} Elf64_SectionHeader;

typedef struct {
    u32     type;
    u32     flags;
    u64     offset;
    u64     vaddr;
    u64     paddr;
    u64     filesize;
    u64     memsize;
    u64     alignment;
} Elf64_ProgramHeader;

typedef struct {
    i64     tag;
    union   {
        u64     val;
        u64     ptr;
    };
} Elf64_Dynamic;

typedef struct {
    u32     name;
    u8      info;
    u8      other;
    u16     section_index;
    u64     value;
    u64     size;
} Elf64_Symbol;

#endif
