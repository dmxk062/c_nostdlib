#pragma once
#ifndef _PATH_H
#define _PATH_H

#include "types.h"
#include "string.h"
#include "filesystem.h"


typedef struct PathList {
    Vec(String)* path;
} PathList;

PathList* PathList_new_from_zstr(zstr list, char sep);
PResult(String) PathList_lookup_name(PathList* list, String* name, enum AccessMode mode);
void PathList_free(PathList* list);


#endif
