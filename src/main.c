#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/structures.h"

int main() {
    // Test code, meant to be a quick showcase, not anything memory-safe and/or efficient
    // Haiiiii btw :3

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

    __uint64_t uint64_val;
    data_get(uint64_type, &uint64_val);
    printf("%ld\n", uint64_val);

    float float_val;
    data_get(float_type, &float_val);
    printf("%f\n", float_val);

    // DataArray has to be initialized as empty,
    // otherwise it's values will be random
    DataArray array = {};

    // We can add multiple values at once
    arr_add_all(&array, (Data[]){uint64_type, string_type, float_type});
    // And insert into a specific index
    arr_insert(&array, bool_type, 0);
    
    DataArray new_array = {};

    arr_add(&new_array, uint64_type);
    arr_add(&new_array, string_type);

    // We can also put an array into an array by setting it as Data
    arr_add(&array, data_init(ARRAY, &new_array));

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
    // And we can clear the array fully
    arr_clear(&array);
    printf("\narray is empty: %s\n\n", arr_is_empty(array) ? "true" : "false");

    Data *all_values = arr_get_values(new_array);
    for (uint64_t i = 0; i < arr_size(new_array); i++) {
        data_printf("%%\n", all_values[i]);
    }
    
    // Hashmap test
    printf("\nExample of hashmap:\n");

    // We can create a hashmap and put some values in it
    HashMap map = {};
    for (__uint64_t i = 0; i < 15; i++) {
        // Hashmaps take arrays as input
        DataArray arr = {};
        arr_add(&arr, data_init(UINT64_T, (__uint64_t[]){i*65535}));
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

        // We can put a key, value pair into the hashmap using put
        hm_put(&map, key, arr);
    }
    hm_put(&map, "my lovely key", new_array);

    // We can visualize the hashmap
    hm_tree(map);

    // We can also get all the keys and values from it
    printf("\nExample of get_keys and get_values\n");
    char* *keys = hm_get_keys(map);
    DataArray *values = hm_get_values(map);
    for (__uint64_t i = 0; i < hm_size(map); i++) {
        printf("%s - ", keys[i]);
        data_printf("%%\n", data_init(ARRAY, &values[i]));
    }

    // We can check if a hashmap contains a key with has_key
    printf("\nExample of has_key\n");
    printf("my lovely key - %s\n", hm_has_key(map, "my lovely key") ? "true" : "false");
    printf("my lovely key that doesn't exist - %s\n", hm_has_key(map, "my lovely key that doesnt exist") ? "true" : "false");

    // And if a hashmap contains a value with has_value
    printf("\nExample of has_value\n");
    printf("array - %s\n", hm_has_value(map, array) ? "true" : "false");
    printf("new_array - %s\n", hm_has_value(map, new_array) ? "true" : "false");

    // We can also get a value from a key
    DataArray returnArray = hm_get(map, "my lovely key");
    data_printf("\nValue at my lovely key: %%\n", data_init(ARRAY, &returnArray));

    // Clear the hashmap
    hm_clear(&map);

    // We can also put multiple values into the hashmap at once
    // Note that the key list has to end with the last string as null
    hm_put_all(&map, (char*[]){"uno", "dos", "tres", NULL}, (DataArray[]){new_array, new_array, array});

    printf("\n");
    hm_tree(map);

    // Now delete all of the following keys
    // Yet again, the key list has to end with NULL
    hm_del_all(&map, (char*[]){"uno", "dos", "tres", NULL});

    printf("\n");
    hm_tree(map);
    
    return 0;
}