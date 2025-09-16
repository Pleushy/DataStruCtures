#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

int main() {
    // Test code :3

    // Dynamic types and arrays
    DataType uint64_type = init(UINT64_T, &(__uint64_t){65535});
    DataType string_type = init(STRING, "Hello, world!");
    DataType float_type = init(FLOAT, &(float){3.14159f});
    DataType bool_type = init(BOOL, &(bool){true});

    // We can get a value back by using get_value
    char* *value = get_value(string_type);
    printf("%s\n\n", *value);

    // DataTypeArray has to be initialized as empty,
    // otherwise it's values will be random
    DataTypeArray array = {};

    put(&array, uint64_type);
    put(&array, string_type);
    put(&array, float_type);
    put(&array, bool_type);

    // If we go beyond the size of the array,
    // the values are NULL
    for (int i = 0; i < 5; i++) {
        DataType current_type = get(array, i);
        print("Value: %% | Type: ", current_type);
        printf("%s\n", type(current_type));
    }
    // NULL should be avoided, otherwise
    // it may lead to unexpected behavior
    printf("\n");

    // Deletes data type at first index of array
    del(&array, 0);

    for (int i = 0; i < 5; i++) {
        DataType current_type = get(array, i);
        print("Value: %% | Type: ", current_type);
        printf("%s\n", type(current_type));
    }
    printf("\n");

    // Hashmap (TODO)
    HashMapObject obj[] = {hashmap_object_init("myKey", array)};
    HashMap map = hashmap_init(obj, 1);
    HashMapObject newObj = hashmap_get(map, "myKey");
    printf("%s %s\n", map.objects[0].key, *(char**)get_value(get(newObj.val, 0)));

    return 0;
}