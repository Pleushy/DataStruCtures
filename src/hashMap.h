#include "dynamicType.h"
#include "sha256.h"

typedef struct {
    struct {
        char *key;
        DataArray array;
        __uint64_t hash;
    } *objects;
    __uint64_t size;
    __uint64_t limit;
} HashMap;

void hm_put(HashMap *map, char *key, DataArray array);

void hm_del(HashMap *map, char *key);

DataArray hm_get(HashMap map, char *key);