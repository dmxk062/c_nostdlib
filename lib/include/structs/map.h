// vim: ft=c
#ifndef _STRUCTS_MAP_H
#define _STRUCTS_MAP_H
#include "types.h"
#include "structs/destructor.h"

/*
 * Provides a simple hashmap structure
 */


struct MapEntry {
    struct MapEntry* next;

    u64     hash;
    untyped value;
};

typedef struct {
    struct MapEntry* first;

    // function that will be called when an entry in the Map is deleted or the map is destroyed
    EntryDestructor on_destroy;
} Map;



Map* Map_new(void);

errno_t Map_set(Map* map, char* key, u64 key_len, untyped value);
errno_t Map_zset(Map* map, zstr key, untyped value);

Result(untyped) Map_get(Map* map, char* key, u64 key_len);
Result(untyped) Map_zget(Map* map, zstr key);

errno_t Map_del(Map* map, char* key, u64 key_len);
errno_t Map_zdel(Map* map, zstr key);

errno_t Map_free(Map* map);


#endif
