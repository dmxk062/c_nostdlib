#pragma once
#ifndef _PATH_H
#define _PATH_H

#include "types.h"
#include "string.h"


struct PathAccess {
    StringList* path;
};


errno_t find_in_path(struct PathAccess* path, String out, String search, u64 mode);

#endif
