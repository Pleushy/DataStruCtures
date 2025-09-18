#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/hashMap.h"

// gets a value from a hash map
DataArray hm_get(HashMap map, char *key) {
    if (!map.limit) return (DataArray){};

    __uint64_t hash = create_hash(key);
    __uint64_t index = hash%map.limit;

    return map.objects[index].array;
}

void hm_put(HashMap *map, char *key, DataArray array) {
    if (!map->limit) {
        map->limit++;
        map->objects = malloc(sizeof(_HashMapObject));
    }

    __uint64_t byte_size = sizeof(_HashMapObject)*map->limit;
    _HashMapObject *tmp = malloc(byte_size);
    if (tmp == NULL) return;
    memcpy(tmp, map->objects, byte_size);

    if ((map->size+1) >= map->limit) {
        map->limit *= 2;
        
        map->objects = realloc(map->objects, sizeof(_HashMapObject)*map->limit);
        if (map->objects == NULL) return;
        memset(map->objects,0,byte_size);

        for (__uint64_t i = 0; i < map->limit/2; i++) {
            map->objects[tmp[i].hash%map->limit] = tmp[i];
        }
    } else {
        memcpy(map->objects, tmp, byte_size);
    }
    free(tmp);

    __uint64_t hash = create_hash(key);
    __uint64_t index = hash%map->limit;

    map->objects[index].array = array;
    map->objects[index].hash = hash;
    map->objects[index].key = key;
    map->size++;
}

void hm_del(HashMap *map, char *key) {
    if (!map->limit) return;
    if (map->limit == 1) {
        map->limit = 0;
        map->size = 0;
        free(map->objects);
        return;
    }

    __uint64_t byte_size = sizeof(_HashMapObject)*map->limit;
    _HashMapObject *tmp = malloc(byte_size);
    if (tmp == NULL) return;
    memcpy(tmp, map->objects, byte_size);

    if ((map->size-1) < (map->limit/2-1)) {
        map->limit /= 2;
        
        map->objects = realloc(map->objects, sizeof(_HashMapObject)*map->limit);
        if (map->objects == NULL) return;
        memset(map->objects,0,byte_size);

        __uint64_t hash = create_hash(key);
        for (__uint64_t i = 0; i < map->limit*2; i++) {
            if (tmp[i].hash == hash) {
                tmp[i] = (_HashMapObject){};
                continue;
            }
            map->objects[tmp[i].hash%map->limit] = tmp[i];
        }
    } else {
        memcpy(map->objects, tmp, byte_size);
        __uint64_t hash = create_hash(key);
        __uint64_t index = hash%map->limit;

        map->objects[index] = (_HashMapObject){};
    }
    free(tmp);

    map->size--;
}