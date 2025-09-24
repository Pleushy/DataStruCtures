#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/hashMap.h"

static void memfail() {
    fprintf(stderr, "Failed to allocate memory\n");
    return;
}

void bucket_add(HashBucket *bucket, HashMapObject obj) {
    if (!bucket->limit) {
        bucket->limit++;
        bucket->objects = malloc(sizeof(HashMapObject));
        if (bucket->objects == NULL) {
            memfail();
            return;
        }
    } else {
        // check if exists
        // use bucket->size, not bucket->limit (cost me 10 mins of debugging)
        for (uint64_t i = 0; i < bucket->size; i++) {
            if (!strcmp(bucket->objects[i].key, obj.key)) {
                return;
            }
        }
    }

    uint64_t byte_size = sizeof(HashMapObject)*bucket->limit;
    void *tmp = malloc(byte_size);
    if (tmp == NULL) {
        memfail();
        return;
    }
    memcpy(tmp, bucket->objects, byte_size);

    if ((bucket->size+1) >= bucket->limit) {
        bucket->limit *= 2;
        
        bucket->objects = realloc(bucket->objects, sizeof(HashMapObject)*bucket->limit);
        if (bucket->objects == NULL) {
            memfail();
            return;
        }
    }
    memcpy(bucket->objects, tmp, byte_size);

    free(tmp);
    tmp = NULL;

    bucket->objects[bucket->size] = obj;
    bucket->size++;
}

void bucket_del(HashBucket *bucket, char *key) {
    if (!bucket->limit) return;
    if (!(bucket->size-1)) {
        bucket->size = 0;
        bucket->limit = 0;
        free(bucket->objects);
        bucket->objects = NULL;
        return;
    }

    uint64_t byte_size = sizeof(HashMapObject)*bucket->limit;
    HashMapObject *tmp = malloc(byte_size);
    if (tmp == NULL) {
        memfail();
        return;
    }
    memcpy(tmp, bucket->objects, byte_size);

    if ((bucket->size-1) <= (bucket->limit/2-1)) {
        bucket->limit /= 2;
        
        bucket->objects = realloc(bucket->objects, sizeof(HashMapObject)*bucket->limit);
        if (bucket->objects == NULL) {
            memfail();
            return;
        }
    }

    bool found = false;
    for (uint64_t i = 0; i < bucket->size; i++) {
        if (!strcmp(bucket->objects[i].key, key) && !found) {
            found = true;
            continue;
        }
        bucket->objects[i-found] = tmp[i];
    }

    free(tmp);
    tmp = NULL;
    bucket->size--;
}

// gets a value from a hash map
DataArray hm_get(HashMap map, char *key) {
    if (!map.limit) return (DataArray){};

    uint64_t hash = create_hash(key);
    uint64_t index = hash%map.limit;

    for (uint64_t i = 0; i < map.buckets[index].size; i++) {
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
        map->buckets = malloc(sizeof(HashBucket)*map->limit);
        if (map->buckets == NULL) {
            memfail();
            return;
        }
    }

    uint64_t byte_size = sizeof(HashBucket)*map->limit;
    HashBucket *tmp = malloc(byte_size);
    if (tmp == NULL) {
        memfail();
        return;
    }
    memcpy(tmp, map->buckets, byte_size);

    if ((map->size+1) >= map->limit) {
        map->limit *= 2;
        
        map->buckets = realloc(map->buckets, sizeof(HashBucket)*map->limit);
        if (map->buckets == NULL) {
            memfail();
            return;
        }
        
        memset(map->buckets,0,byte_size);

        for (uint64_t i = 0; i < map->limit/2; i++) {
            // have to reorganize every object in every bucket, inefficient and pain but theres no other way (probably)
            if (tmp[i].hash) { // null check
                for (uint64_t j = 0; j < tmp[i].size; j++) {
                    uint64_t newHash = create_hash(tmp[i].objects[j].key);
                    uint64_t newIndex = newHash%map->limit;
                    map->buckets[newIndex].hash = newHash;
                    bucket_add(&map->buckets[newIndex], tmp[i].objects[j]);
                }
            }
        }
    } else {
        memcpy(map->buckets, tmp, byte_size);
    }
    free(tmp);
    tmp = NULL;

    uint64_t hash = create_hash(key);
    uint64_t index = hash%map->limit;

    map->buckets[index].hash = hash;
    bucket_add(&map->buckets[index], (HashMapObject){key,array});
    map->size++;
}

void hm_clear(HashMap *map) {
    free(map->buckets);
    map->buckets = NULL;
    map->size = 0;
    map->limit = 0;
}

void hm_del(HashMap *map, char *key) {
    if (!map->limit) return;
    if (!(map->size-1)) {
        hm_clear(map);
        return;
    }

    uint64_t byte_size = sizeof(HashBucket)*map->limit;
    HashBucket *tmp = malloc(byte_size);
    if (tmp == NULL) {
        memfail();
        return;
    }
    memcpy(tmp, map->buckets, byte_size);

    if ((map->size-1) <= (map->limit/2-1)) {
        map->limit /= 2;
        
        map->buckets = realloc(map->buckets, sizeof(HashBucket)*map->limit);
        if (map->buckets == NULL) {
            memfail();
            return;
        }
        memset(map->buckets,0,byte_size);

        for (uint64_t i = 0; i < map->limit*2; i++) {
            // have to reorganize every object in every bucket, inefficient and pain but theres no other way (probably)
            if (tmp[i].hash) { // this has to be here otherwise it can cause seg fault
                for (uint64_t j = 0; j < tmp[i].size; j++) {
                    if (!strcmp(tmp[i].objects[j].key, key)) {
                        if (!(tmp[i].size-1)) {
                            free(tmp[i].objects);
                            tmp[i].objects = NULL;
                        }
                        continue;
                    }
                    uint64_t newHash = create_hash(tmp[i].objects[j].key);
                    uint64_t newIndex = newHash%map->limit;
                    map->buckets[newIndex].hash = newHash;
                    bucket_add(&map->buckets[newIndex], tmp[i].objects[j]);
                }
            }
        }
    } else {
        uint64_t hash = create_hash(key);
        uint64_t index = hash%map->limit;
        bucket_del(&map->buckets[index], key);
    }
    free(tmp);
    tmp = NULL;
    map->size--;
}

void hm_tree(HashMap map) {
    uint64_t obj_count = 0;
    uint64_t bucket_count = 0;
    for (uint64_t i = 0; i < map.limit; i++) {
        if (map.buckets[i].hash) {
            bucket_count++;
            obj_count+=map.buckets[i].size;
        }
    }
    printf("Map (%ld keys)\n", obj_count);
    for (uint64_t i = 0; i < map.limit; i++) {
        if (!map.buckets[i].hash) bucket_count++;
        for (uint64_t j = 0; j < map.buckets[i].size; j++) {
            if (j == 0) {
                printf(i == bucket_count-1 ? "└" : "├");
                printf("── B%ld\n",i);
            }
            printf(i < bucket_count-1 ? "│" : " ");
            printf("   ");
            printf(j == map.buckets[i].size-1 ? "└" : "├");
            printf("── %s\n", map.buckets[i].objects[j].key);
            for (uint64_t k = 0; k < map.buckets[i].objects[j].array.size; k++) {
                printf(i < bucket_count-1 ? "│" : " ");
                printf("   ");
                printf(j < map.buckets[i].size-1 ? "│" : " ");
                printf("   ");
                printf(k == map.buckets[i].objects[j].array.size-1 ? "└" : "├");
                data_printf("── %%\n", arr_get(map.buckets[i].objects[j].array, k));
            }
        }
    }
}

uint64_t hm_size(HashMap map) {
    uint64_t obj_count = 0;
    for (uint64_t i = 0; i < map.limit; i++) {
        if (map.buckets[i].hash) {
            obj_count+=map.buckets[i].size;
        }
    }
    return obj_count;
}


bool hm_is_empty(HashMap map) {
    return !map.size;
}

char* *hm_get_keys(HashMap map) {
    char* *keys = malloc(map.size*sizeof(char*));
    if (keys == NULL) {
        memfail();
        return NULL;
    }
    uint64_t count = 0;
    for (uint64_t i = 0; i < map.limit; i++) {
        if (map.buckets[i].hash) {
            for (uint64_t j = 0; j < map.buckets[i].size; j++) {
                keys[count++] = map.buckets[i].objects[j].key;
            }
        }
    }
    return keys;
}

DataArray *hm_get_values(HashMap map) {
    DataArray *values = malloc(map.size*sizeof(DataArray));
    if (values == NULL) {
        memfail();
        return NULL;
    }
    uint64_t count = 0;
    for (uint64_t i = 0; i < map.limit; i++) {
        if (map.buckets[i].hash) {
            for (uint64_t j = 0; j < map.buckets[i].size; j++) {
                values[count++] = map.buckets[i].objects[j].array;
            }
        }
    }
    return values;
}

bool hm_has_key(HashMap map, char *key) {
    return hm_get(map, key).size > 0;
}

bool hm_has_value(HashMap map, DataArray value) {
    for (uint64_t i = 0; i < map.limit; i++) {
        if (map.buckets[i].hash) {
            for (uint64_t j = 0; j < map.buckets[i].size; j++) {
                if (map.buckets[i].objects[j].array.data == value.data) {
                    return true;
                }
            }
        }
    }
    return false;
}

void hm_put_all(HashMap *map, char **keys, DataArray* arrays) {
    uint64_t i = 0;
    while (keys[i] != NULL) {
        if (!arrays[i].size) {
            // disregard the next keys, as they will not have values therefore are NULL
            return;
        }
        hm_put(map, keys[i], arrays[i]);
        i++;
    }
}

void hm_del_all(HashMap *map, char** keys) {
    uint64_t i = 0;
    while (keys[i] != NULL) {
        // kill, kill, kill, kill
        hm_del(map, keys[i]);
        i++;
    }
}