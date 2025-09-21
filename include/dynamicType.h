#include <stdint.h>
#include <stdbool.h>

// Defines enums for different data types
typedef enum {
    BOOL,
    STRING,
    UINT64_T,
    FLOAT,
    ARRAY
} DataType;

// Defines data array strcuture in advance so data struct can use it (Array nesting)
struct DATA_ARRAY;

// Defines data structure
typedef struct {
    union {
        bool boolean;
        char *string;
        __uint64_t uint64_val;
        float float_val;
        struct DATA_ARRAY *array_val;
    } data;
    DataType data_enum;
} Data;

// Defines data array structure
typedef struct DATA_ARRAY {
    Data *data;
    __uint64_t size;
    __uint64_t limit;
} DataArray;


// Returns the data type enum in string
char *data_typeof(Data data);

// Returns value of data type
void data_get(Data data, void *out);

// Prints string and value of data type
// Can use "%%" to specify where to place data type
void data_printf(const char* string, Data data);

// Initiates a data type with value and data enum
Data data_init(DataType data_enum, void *val);

// Adds a data type to a data type array
void arr_put(DataArray *array, Data data);

// Removes a data type from a data array on index
void arr_del(DataArray *array, __uint64_t index);

// Returns a data type from a data array on index
Data arr_get(DataArray array, __uint64_t index);