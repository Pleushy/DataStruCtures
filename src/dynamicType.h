#include <stdbool.h>

// Defining enums for different data types
typedef enum {
    BOOL,
    STRING,
    UINT64_T,
    FLOAT
} DataEnum;

// Defining data union to store values
typedef union {
    bool boolean;
    char *string;
    __uint64_t uint64_val;
    float float_val;
} Data;

// Defining data type structure
typedef struct {
    Data data;
    DataEnum data_enum;
} DataType;

// Defining data type structure array
typedef struct {
    DataType *data_types;
    __uint64_t size;
} DataTypeArray;


// Returns the data type enum in string
char *type(DataType data);

// Prints the data type's value
void print(DataType data);

// Assigns a value and data enum to data type
void assign(DataType *data, DataEnum data_enum, void *val);

// Adds a data type to a data type array
void add(DataTypeArray *array, DataType data);

// Removes a data type from a data array on index
void del(DataTypeArray *array, __uint64_t index);

// Returns a data type from a data array on index
DataType get(DataTypeArray array, __uint64_t index);