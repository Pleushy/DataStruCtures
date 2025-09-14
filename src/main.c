#include <stdio.h>
#include <stdlib.h>
#include "dynamicType.h"

int main() {
    // Test code :3

    DataType uint64_type;
    DataType string_type;
    DataType float_type;
    DataType bool_type;

    assign(&uint64_type, UINT64_T, &(__uint64_t){65535});
    assign(&string_type, STRING, "Hello, world!");
    assign(&float_type, FLOAT, &(float){3.14159f});
    assign(&bool_type, BOOL, &(bool){true});

    // DataTypeArray has to be initialized as empty,
    // otherwise it's values will be random
    DataTypeArray array = {};

    add(&array, uint64_type);
    add(&array, string_type);
    add(&array, float_type);
    add(&array, bool_type);

    // If we go beyond the size of the array,
    // the values are NULL
    for (int i = 0; i < 5; i++) {
        DataType current_type = get(array, i);
        print(current_type);
        printf(" >> %s\n", type(current_type));
    }
    // NULL should be avoided, otherwise
    // unexpected things may happen

    return 0;
}