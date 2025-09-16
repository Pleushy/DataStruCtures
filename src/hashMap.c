#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashMap.h"

HashMapObject hashmap_object_init(char *key, DataTypeArray val) {
    HashMapObject new = {key, val, create_hash(key)};
    return new;
}

HashMap hashmap_init(HashMapObject *objects, __uint64_t size) {
    HashMap new = {objects, size};
    return new;
}

HashMapObject hashmap_get(HashMap map, char *key) {
    __uint64_t newHash = create_hash(key);
    __uint64_t index = newHash % map.size;

   HashMapObject returnVal = {};
    if (map.size < index) return returnVal;
    return map.objects[index];
}