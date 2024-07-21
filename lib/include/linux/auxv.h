#pragma once
#ifndef _LINUX_AUXV_H
#define _LINUX_AUXV_H
#include "types.h"


enum AuxvalType {
    AuxvalType_NULL     = 0,
    AuxvalType_IGNORE   = 1,
    AuxvalType_EXECFD   = 2, 
    AuxvalType_PHEADER  = 3, /* Program header */
    AuxvalType_EHEADER  = 4, /* Header entry point */
    AuxvalType_NHEADER  = 5, /* Header count */
    AuxvalType_PAGESIZE = 6,
    AuxvalType_BASE     = 7,
    AuxvalType_FLAGS    = 8,
    AuxvalType_ENTRY    = 9,
    AuxvalType_NOTELF   = 10,
    AuxvalType_UID      = 11,
    AuxvalType_EUID     = 12,
    AuxvalType_GID      = 13,
    AuxvalType_GUID     = 14,

    AuxvalType_CLOCKTICK = 17,

    AuxvalType_PLATFORM  = 15,
    AuxvalType_HWCAP     = 16,
    AuxvalType_FPUCW     = 18,

    AuxvalType_EXECFN    = 31, /* Filename of executable */
    AuxvalType_VDSO_ADDRESS = 33,
};

typedef struct {
    enum AuxvalType type;
    union {
        untyped value;
    };
} Elf64Auxval;

Elf64Auxval* Auxval_find_auxv(zstr* envp);
Elf64Auxval* Auxval_get_value(Elf64Auxval* auxvector, enum AuxvalType type);




#endif
