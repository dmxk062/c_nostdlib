#include "structs/map.h"
#include "process.h"
#include "types.h"
#include "cstring.h"
#include <io.h>
#include "alloc.h"

static u64 hash_key(char* string, u64 len) {
    u64 hash = 5381;
    for (u64 i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) + string[i];
    }

    return hash;
}


Map* Map_new(void) {
    Map* map = malloc(sizeof(Map));
    if (!map) return NULL;

    map->first = NULL;
    map->default_destroy = NULL;

    return map;
}


errno_t Map_set(Map* map, char* key, u64 key_len, untyped value) {
    struct MapEntry *entry = map->first, *last = entry;
    u64 hash = hash_key(key, key_len);

    // new map
    if (!map->first) {
        struct MapEntry* new_entry = malloc(sizeof(struct MapEntry));
        if (!new_entry) {
            return 1;
        }

        new_entry->value = value;
        new_entry->hash  = hash;
        new_entry->next  = NULL;
        new_entry->destroy = map->default_destroy;
        map->first = new_entry;

        return 0;

    }

    while (entry) {
        if (entry->hash == hash) {
            if (entry->destroy) {
                entry->destroy(entry->value);
            }
            entry->value = value;
            return 0;
        } 

        last  = entry;
        entry = entry->next;
    }

    // append
    struct MapEntry* new_entry = malloc(sizeof(struct MapEntry));
    if (!new_entry) {
        return 1;
    }

    new_entry->value = value;
    new_entry->hash  = hash;
    new_entry->next  = NULL;
    new_entry->destroy = map->default_destroy;

    last->next = new_entry;

    return 0;
}

untyped Map_get(Map* map, char* key, u64 key_len) {
    struct MapEntry* entry = map->first;
    u64 hash = hash_key(key, key_len);

    while (entry) {
        if (entry->hash == hash) {
            return entry->value;
        }
        entry = entry->next;
    }

    return NULL;

}

errno_t Map_del(Map* map, char* key, u64 key_len) {
    struct MapEntry *entry = map->first, *last = entry;
    u64 hash = hash_key(key, key_len);

    while (entry) {
        if (entry->hash == hash) {
            if (entry->destroy) {
                entry->destroy(entry);
            }
            last->next = entry->next;
            free(entry);
            return 0;
        }
        last  = entry;
        entry = entry->next;
    }

    return 1;
}

errno_t Map_zset(Map* map, zstr key, untyped value) {
    return Map_set(map, key, strlen(key), value);
}

untyped Map_zget(Map* map, zstr key) {
    return Map_get(map, key, strlen(key));
}

errno_t Map_zdel(Map* map, zstr key) {
    return Map_del(map, key, strlen(key));
}


errno_t Map_free(Map* map) {
    struct MapEntry* entry = map->first;

    while (entry) {
        if (entry->destroy) {
            entry->destroy(entry->value);
        }
        free(entry);
        entry = entry->next;
    }

    free(map);

    return 0;
}
