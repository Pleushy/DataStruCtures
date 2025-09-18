#include "dynamicType.h"
#include "sha256.h"

// ONLY USED INTERNALLY, DO NOT USE
typedef struct {
    char *key;
    DataArray array;
    __uint64_t hash;
} _HashMapObject;

// Defines an empty hashmap
typedef struct {
    _HashMapObject *objects;
    __uint64_t size;
    __uint64_t limit;
} HashMap;

// Puts a key, value pair in hashmap
void hm_put(HashMap *map, char *key, DataArray array);

// Removes a key in hashmap
void hm_del(HashMap *map, char *key);

// Prints all key, value pairs from hashmap (TODO)
void hm_tree(HashMap map);

// Retrieves value from key
DataArray hm_get(HashMap map, char *key);