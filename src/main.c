#include <stdio.h>
#include <stdlib.h>
#include "../include/structures.h"

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

    // Hashmap test
    HashMap map = {};
    for (__uint64_t i = 0; i < 64; i++) {
        DataArray arr = {};
        arr_put(&arr, data_init(UINT64_T, &(__uint64_t){i}));
        char *key;
        if (i) {
            key = malloc(i/10+1);
            char *tmp = malloc(i/10+1);
            __uint64_t j = 0;
            __uint64_t k = i;
            while(k > 0) {
                tmp[j++] = k%10+'0';
                k/=10;
            }
            __uint64_t l = 0; 
            for (;l < j; l++) {
                key[l] = tmp[j-1-l];
            }
            key[l] = '\0';
        } else {
            key = malloc(2);
            key[0] = '0';
            key[1] = '\0';
        }
        hm_put(&map, key, arr);
        printf("KEY: %s", key);
        data_printf(" | VALUE: %%\n", arr_get(hm_get(map, key), 0));
    }
    // todo: implement
    // hm_tree(map);

    return 0;
}