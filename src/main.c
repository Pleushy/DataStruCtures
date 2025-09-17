#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

int main() {
    // Test code :3

    // Dynamic types and arrays
    Data uint64_type = data_init(UINT64_T, &(__uint64_t){65535});
    Data string_type = data_init(STRING, "Hello, world!");
    Data float_type = data_init(FLOAT, &(float){3.14159f});
    Data bool_type = data_init(BOOL, &(bool){true});

    // We can get a value back by using data_get
    void* value = data_get(string_type);
    printf("%s\n\n", *(char**)value);

    // DataArray has to be initialized as empty,
    // otherwise it's values will be random
    DataArray array = {};

    arr_put(&array, uint64_type);
    arr_put(&array, string_type);
    arr_put(&array, float_type);
    arr_put(&array, bool_type);
    
    DataArray newArray = {};

    arr_put(&newArray, uint64_type);
    arr_put(&newArray, string_type);

    arr_put(&array, data_init(ARRAY, &newArray));

    // If we go beyond the size of the array,
    // the values are NULL
    for (int i = 0; i < 5; i++) {
        Data current_type = arr_get(array, i);
        data_printf("Value: %% | Type: ", current_type);
        printf("%s\n", data_typeof(current_type));
    }
    
    // NULL should be avoided, as it may lead to
    // unexpected behavior
    printf("\n");

    // Deletes data type at first index of array
    arr_del(&array, 0);

    for (int i = 0; i < 5; i++) {
        Data current_type = arr_get(array, i);
        data_printf("Value: %% | Type: ", current_type);
        printf("%s\n", data_typeof(current_type));
    }
    printf("\n");

    // Hashmap - work in progress
    HashMap map = {};
    hm_put(&map, "1", array);
    hm_put(&map, "2", newArray);

    DataArray retArray = hm_get(map, "1");
    DataArray retArray2 = hm_get(map, "2");
    data_printf("1 - %%\n", data_init(ARRAY, &retArray));
    data_printf("2 - %%\n", data_init(ARRAY, &retArray2));

    return 0;
}