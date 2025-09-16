#include "dynamicType.h"
#include "sha256.h"

typedef struct {
    char *key;
    DataTypeArray val;
    __uint64_t hash;
} HashMapObject;

typedef struct {
    HashMapObject *objects;
    __uint64_t size;
} HashMap;

HashMapObject hashmap_object_init(char *key, DataTypeArray val);

HashMap hashmap_init(HashMapObject *objects, __uint64_t size);

void hashmap_put(HashMap map, char *key, DataType val);

void hashmap_del(HashMap map, char *key);

HashMapObject hashmap_get(HashMap map, char *key);