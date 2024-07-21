#pragma once
#ifndef _LINUX_VDSO_H
#define _LINUX_VDSO_H

#include <types.h>
#include <linux/elf.h>
#include <linux/auxv.h>

address VDSO_find_symbol(address vdso_header, zstr name);

#endif
