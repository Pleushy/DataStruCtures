#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamicType.h"

DataType get(DataTypeArray array, __uint64_t index) {
    if (index > (array.size-1)) {
        DataType returnVal = {-1, -1};
        return returnVal;
    }
    return array.data_types[index];
}

void add(DataTypeArray *array, DataType data) {
    int byte_size = sizeof(DataType)*array->size;
    DataType *tmp = malloc(byte_size);
    memcpy(tmp, array->data_types, byte_size);
    array->data_types = realloc(array->data_types, byte_size+sizeof(DataType));
    if (array->data_types == NULL) {
        printf("Failed to add data -> not enough memory available.");
        return;
    }
    memcpy(array->data_types, tmp, byte_size);
    free(tmp);
    array->data_types[array->size] = data;
    array->size++;
}

void del(DataTypeArray *array, __uint64_t index) {
    int byte_size = sizeof(DataType)*array->size;
    DataType *tmp = malloc(byte_size);
    memcpy(tmp, array->data_types, byte_size);
    array->data_types = realloc(array->data_types, byte_size-sizeof(DataType));
    if (array->data_types == NULL) {
        printf("Failed to delete data -> not enough memory available.");
        return;
    }
    bool found;
    for (int i = 0; i < array->size; i++) {
        if (i == index && !found) {
            found = true;
            continue;
        }
        array->data_types[i-found] = tmp[i];
    }
    free(tmp);
    array->size--;
}

void assign(DataType *data, DataEnum data_enum, void *val) {
    data->data_enum = data_enum;

    switch (data->data_enum) {
        case BOOL:
            data->data.boolean = val;
            break;
        case STRING:
            data->data.string = val;
            break;
        case UINT64_T:
            __uint64_t *tmp_UINT = val;
            data->data.uint64_val = *tmp_UINT;
            break;
        case FLOAT:
            float *tmp_FLOAT = val;
            data->data.float_val = *tmp_FLOAT;
            break;
        
        default:
            printf("Error assigning value");
            break;
    }
}

char *type(DataType data) {
    char *t;
    switch (data.data_enum) {
        case BOOL:
            t = "BOOL";
            break;
        case STRING:
            t = "STRING";
            break;
        case UINT64_T:
            t = "UINT64_T";
            break;
        case FLOAT:
            t = "FLOAT";
            break;
        
        default:
            t = "NULL";
            break;
    }
    return t;
}

void print(DataType data) {
    switch (data.data_enum) {
        case BOOL:
            printf("%d", data.data.boolean);
            break;
        case STRING:
            printf("%s", data.data.string);
            break;
        case UINT64_T:
            printf("%ld", data.data.uint64_val);
            break;
        case FLOAT:
            printf("%f", data.data.float_val);
            break;
        
        default:
            printf("NULL");
            break;
    }
}