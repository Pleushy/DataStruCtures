#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/structures.h"

int main() {
    // Test code :3

    // Dynamic types and arrays
    Data uint64_type = data_init(UINT64_T, (__uint64_t[]){65535});
    Data string_type = data_init(STRING, "Hello, world!");
    Data float_type = data_init(FLOAT, (float[]){3.141592f});
    Data bool_type = data_init(BOOL, (bool[]){true});

    // We can get a value back by using data_get
    bool bool_val;
    data_get( bool_type, &bool_val);
    printf("Example of data_get:\n%d\n",  bool_val);

    char *string_val;
    data_get(string_type, &string_val);
    printf("%s\n", string_val);

    __uint64_t uint64_val = 65535;
    data_get(uint64_type, &uint64_val);
    printf("%ld\n", uint64_val);

    float float_val = 3.141592f;
    data_get(float_type, &float_val);
    printf("%f\n", float_val);

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

    // We can also put an array into an array by setting it as Data
    arr_put(&array, data_init(ARRAY, &newArray));

    // If we go beyond the size of the array,
    // the values are NULL
    printf("\nExample of array with all four values:\n");
    for (int i = 0; i < 5; i++) {
        Data current_type = arr_get(array, i);
        data_printf("Value: %% | Type: ", current_type);
        printf("%s\n", data_typeof(current_type));
    }

    // We can delete the value at the first index
    arr_del(&array, 0);

    printf("\nExample of array with first value removed:\n");
    for (int i = 0; i < 5; i++) {
        Data current_type = arr_get(array, i);
        data_printf("Value: %% | Type: ", current_type);
        printf("%s\n", data_typeof(current_type));
    }

    // Hashmap test
    printf("\nExample of hashmap:\n");

    // We can create a blank hashmap
    HashMap map = {};
    for (__uint64_t i = 0; i < 15; i++) {
        // Hashmaps take arrays as input
        DataArray arr = {};
        arr_put(&arr, data_init(UINT64_T, (__uint64_t[]){i*65535}));
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

        // We can put a key, value pair into the hashmap
        hm_put(&map, key, arr);
    }
    hm_put(&map, "my lovely key", array);

    // We can visualize the hashmap
    hm_tree(map);

    // We can also get a value from a key
    DataArray returnArray = hm_get(map, "5");
    data_printf("%%\n", data_init(ARRAY, &returnArray));
    return 0;
}