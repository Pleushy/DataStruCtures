#include <stdbool.h>
#include "dynamicType.h"
#include "sha256.h"

// ONLY USED INTERNALLY, DO NOT USE
typedef struct {
    char *key;
    DataArray array;
} HashMapObject;

// ONLY USED INTERNALLY, DO NOT USE
typedef struct {
    HashMapObject *objects;
    uint64_t hash;
    uint64_t size;
    uint64_t limit;
} HashBucket;

// Defines an empty hashmap
typedef struct {
    HashBucket *buckets;
    uint64_t size;
    uint64_t limit;
} HashMap;

// Puts a key, value pair in hashmap
void hm_put(HashMap *map, char *key, DataArray array);

// Removes a key in hashmap
void hm_del(HashMap *map, char *key);

// Puts multiple key, value pairs into hashmap
void hm_put_all(HashMap *map, char **keys, DataArray* arrays);

// Deletes multiple keys in hashmap
void hm_del_all(HashMap *map, char** keys);

// Prints all key, value pairs from hashmap (TODO)
void hm_tree(HashMap map);

// Retrieves value from key
DataArray hm_get(HashMap map, char *key);

// Returns size of hashmap
uint64_t hm_size(HashMap map);

// Returns true if hashmap is empty
bool hm_is_empty(HashMap map);

// Clears a hashmap
void hm_clear(HashMap *map);

// Returns every key in hashmap
char* *hm_get_keys(HashMap map);

// Returns every value in hashmap
DataArray *hm_get_values(HashMap map);

// Returns true if hashmap contains key
bool hm_has_key(HashMap map, char *key);

// Returns true if hashmap contains value
bool hm_has_value(HashMap map, DataArray value);