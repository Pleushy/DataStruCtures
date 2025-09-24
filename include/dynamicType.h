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
        uint64_t uint64_val;
        float float_val;
        struct DATA_ARRAY *array_val;
    } data;
    DataType data_enum;
} Data;

// Defines data array structure
typedef struct DATA_ARRAY {
    Data *data;
    uint64_t size;
    uint64_t limit;
} DataArray;

// Returns the data enum in string
char *data_typeof(Data data);

// Returns value of data
void data_get(Data data, void *out);

// Prints string and value of data
// Can use "%%" to specify where to place data
void data_printf(const char* string, Data data);

// Initiates data with value and data enum
Data data_init(DataType data_enum, void *val);

// Adds data to the last index of data array
void arr_add(DataArray *array, Data data);

// Removes data from a data array on index
void arr_del(DataArray *array, uint64_t index);

// Returns data from a data array on index
Data arr_get(DataArray array, uint64_t index);

// Clears an array
void arr_clear(DataArray *array);

// Inserts a data type 
void arr_insert(DataArray *array, Data data, uint64_t index);

// Returns true if array is empty
uint64_t arr_is_empty(DataArray array);

// Returns size of array
uint64_t arr_size(DataArray array);

// Returns a list of all values in array
Data *arr_get_values(DataArray array);

// Puts all of data into array
void arr_add_all(DataArray *array, Data *data);