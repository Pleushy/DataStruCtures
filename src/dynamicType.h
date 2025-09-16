#include <stdbool.h>

// Defines enums for different data types
typedef enum {
    BOOL,
    STRING,
    UINT64_T,
    FLOAT
} DataEnum;

// Defines data union to store values
typedef union {
    bool boolean;
    char *string;
    __uint64_t uint64_val;
    float float_val;
} Data;

// Defines data type structure
typedef struct {
    Data data;
    DataEnum data_enum;
} DataType;

// Defines data type structure array
typedef struct {
    DataType *data_types;
    __uint64_t size;
    __uint64_t limit;
} DataTypeArray;


// Returns the data type enum in string
char *type(DataType data);

// Returns value of data type
void *get_value(DataType data);

// Prints string and value of data type
// Can use "%%" to specify where to place data type
void print(const char* string, DataType data);

// Initiates a data type with value and data enum
DataType init(DataEnum data_enum, void *val);

// Adds a data type to a data type array
void put(DataTypeArray *array, DataType data);

// Removes a data type from a data array on index
void del(DataTypeArray *array, __uint64_t index);

// Returns a data type from a data array on index
DataType get(DataTypeArray array, __uint64_t index);