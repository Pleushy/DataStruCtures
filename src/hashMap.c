#include <iso646.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/hashMap.h"

void _bucket_add(_Bucket *bucket, _HashMapObject obj) {
    if (!bucket->limit) {
        bucket->limit++;
        bucket->objects = malloc(sizeof(_HashMapObject));
    } else {
        // check if exists
        // use bucket->size, not bucket->limit (cost me 10 mins of debugging)
        for (__uint64_t i = 0; i < bucket->size; i++) {
            if (!strcmp(bucket->objects[i].key, obj.key)) {
                return;
            }
        }
    }

    __uint64_t byte_size = sizeof(_HashMapObject)*bucket->limit;
    _HashMapObject *tmp = malloc(byte_size);
    if (tmp == NULL) return;
    memcpy(tmp, bucket->objects, byte_size);

    if ((bucket->size+1) >= bucket->limit) {
        bucket->limit *= 2;
        
        bucket->objects = realloc(bucket->objects, sizeof(_HashMapObject)*bucket->limit);
        if (bucket->objects == NULL) return;
    }
    memcpy(bucket->objects, tmp, byte_size);

    free(tmp);

    bucket->objects[bucket->size] = obj;
    bucket->size++;
}

// _HashMapObject *_bucket_get(_Bucket *bucket, char* key) {
//     for (__uint64_t i = 0; i < bucket->size; i++) {
//         if (!strcmp(bucket->objects[i].key, key)) {
//             return &bucket->objects[i];
//         }
//     }
    
//     return (_HashMapObject*){};
// }

void _bucket_del(_Bucket *bucket, char *key) {
    printf("Removing\n");
    if (!bucket->limit) return;
    if (bucket->limit == 1) {
        bucket->size = 0;
        bucket->limit = 0;
        free(bucket->objects);
        return;
    }

    __uint64_t byte_size = sizeof(_HashMapObject)*bucket->limit;
    _HashMapObject *tmp = malloc(byte_size);
    if (tmp == NULL) return;
    memcpy(tmp, bucket->objects, byte_size);

    if ((bucket->size-1) <= bucket->limit/2-1) {
        bucket->limit /= 2;
        
        bucket->objects = realloc(bucket->objects, sizeof(_HashMapObject)*bucket->limit);
        if (bucket->objects == NULL) return;
    }

    bool found;
    for (__uint64_t i = 0; i < bucket->size; i++) {
        if (!strcmp(bucket->objects[i].key, key) && !found) {
            found = true;
            continue;
        }
        bucket->objects[i-found] = tmp[i];
    }

    free(tmp);
    bucket->size--;
}

// gets a value from a hash map
DataArray hm_get(HashMap map, char *key) {
    if (!map.limit) return (DataArray){};

    __uint64_t hash = create_hash(key);
    __uint64_t index = hash%map.limit;

    for (__uint64_t i = 0; i < map.buckets[index].size; i++) {
        // compare keys
        if (!strcmp(map.buckets[index].objects[i].key, key)) {
            return map.buckets[index].objects[i].array;
        }
    }

    // return empty if not found
    return (DataArray){};
}

void hm_put(HashMap *map, char *key, DataArray array) {
    if (!map->limit) {
        map->limit++;
        map->buckets = malloc(sizeof(_Bucket)*map->limit);
    }

    __uint64_t byte_size = sizeof(_Bucket)*map->limit;
    _Bucket *tmp = malloc(byte_size);
    if (tmp == NULL) return;
    memcpy(tmp, map->buckets, byte_size);

    if ((map->size+1) >= map->limit) {
        map->limit *= 2;
        
        map->buckets = realloc(map->buckets, sizeof(_Bucket)*map->limit);
        if (map->buckets == NULL) return;
        memset(map->buckets,0,byte_size);

        for (__uint64_t i = 0; i < map->limit/2; i++) {
            bool changed = false;
            for (__uint64_t j = 0; j < tmp[i].size; j++) {
                __uint64_t newIndex = create_hash(tmp[i].objects[j].key)%map->limit;
                if (newIndex != i) {
                    char *k = tmp[i].objects[j].key;
                    DataArray a = tmp[i].objects[j].array;
                    _bucket_del(&tmp[i], k);
                    map->buckets[tmp[newIndex].hash%map->limit] = tmp[i];
                    _bucket_add(&map->buckets[newIndex], (_HashMapObject){k,a});
                    changed = true;
                }
            }
            if (changed) continue;
            map->buckets[tmp[i].hash%map->limit] = tmp[i];
        }
    } else {
        memcpy(map->buckets, tmp, byte_size);
    }
    free(tmp);

    __uint64_t hash = create_hash(key);
    __uint64_t index = hash%map->limit;

    map->buckets[index].hash = hash;
    _bucket_add(&map->buckets[index], (_HashMapObject){key,array});
    map->size++;
}

// void hm_del(HashMap *map, char *key) {
//     if (!map->limit) return;
//     if (map->limit == 1) {
//         map->limit = 0;
//         map->size = 0;
//         free(map->buckets);
//         return;
//     }

//     __uint64_t byte_size = sizeof(_Bucket)*map->limit;
//     _Bucket *tmp = malloc(byte_size);
//     if (tmp == NULL) return;
//     memcpy(tmp, map->buckets, byte_size);

//     if ((map->size-1) <= (map->limit/2-1)) {
//         map->limit /= 2;
        
//         map->buckets = realloc(map->buckets, sizeof(_Bucket)*map->limit);
//         if (map->buckets == NULL) return;
//         memset(map->buckets,0,byte_size);

//         __uint64_t hash = create_hash(key);
//         for (__uint64_t i = 0; i < map->limit*2; i++) {
//             if (tmp[i].hash == hash) {
//                 tmp[i] = (_Bucket){};
//                 continue;
//             }
//             // map->buckets[tmp[i].hash%map->limit] = tmp[i];
//         }
//     } else {
//         memcpy(map->buckets, tmp, byte_size);
//         __uint64_t hash = create_hash(key);
//         __uint64_t index = hash%map->limit;

//         map->buckets[index] = (_Bucket){};
//     }
//     free(tmp);

//     map->size--;
// }

// void hm_tree(HashMap map) {
//     for (__uint64_t i = 0; i < map.limit; i++) {
//         if (map.buckets[i].hash) {
//             // printf("%s\n", map.buckets[i].key);
//         }
//     }
// }