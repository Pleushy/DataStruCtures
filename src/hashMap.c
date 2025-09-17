#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashMap.h"

// gets a value from a hash map
DataArray hm_get(HashMap map, char *key) {
    if (!map.size) return (DataArray){};

    __uint64_t hash = create_hash(key);
    __uint64_t index = !map.size ? 0 : hash%map.size;

    return map.objects[index].array;
}

void hm_put(HashMap *map, char *key, DataArray array) {
    if (!map->limit) map->limit++;

    __uint64_t hash = create_hash(key);
    __uint64_t index = !map->size ? 0 : hash%map->size;

    // byte size is fixed at 64 (smallest value that works :3)
    __uint64_t byte_size = 64*map->size;
    void *tmp = malloc(byte_size);
    memcpy(tmp, map->objects, byte_size);

    if ((map->size+1) >= map->limit) {
        map->limit *= 2;
        
        map->objects = realloc(map->objects, 64*map->limit);
        if (map->objects == NULL) {
            return;
        }
    }
    memcpy(map->objects, tmp, byte_size);
    free(tmp);

    map->objects[index].array = array;
    map->objects[index].hash = hash;
    map->objects[index].key = key;
    map->size++;
}