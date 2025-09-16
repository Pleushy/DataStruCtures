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

void put(DataTypeArray *array, DataType data) {
    if (!array->limit) array->limit++;

    __uint64_t byte_size = sizeof(DataType)*array->size;
    DataType *tmp = malloc(byte_size);
    memcpy(tmp, array->data_types, byte_size);

    if ((array->size+1) >= array->limit) {
        array->limit *= 2;
        //printf("%ld\n", array->limit);
        
        array->data_types = realloc(array->data_types, sizeof(DataType)*array->limit);
        if (array->data_types == NULL) {
            //printf("Failed to put data -> not enough memory available.\n");
            return;
        }
    }

    memcpy(array->data_types, tmp, byte_size);
    free(tmp);
    array->data_types[array->size] = data;
    array->size++;
}

void del(DataTypeArray *array, __uint64_t index) {
    if (index > (array->size-1)) {
        //printf("Index out of bounds.\n");
        return;
    }

    __uint64_t byte_size = sizeof(DataType)*array->size;
    DataType *tmp = malloc(byte_size);
    memcpy(tmp, array->data_types, byte_size);

    if ((array->size+1) < array->limit) {
        array->limit /= 2;
        //printf("%ld\n", array->limit);

        array->data_types = realloc(array->data_types, sizeof(DataType)*array->limit);
        if (array->data_types == NULL) {
            //printf("Failed to delete data -> not enough memory available.\n");
            return;
        }
    }

    bool found;
    for (__uint64_t i = 0; i < array->size; i++) {
        if (i == index && !found) {
            found = true;
            continue;
        }
        array->data_types[i-found] = tmp[i];
    }
    free(tmp);
    array->size--;
}

DataType init(DataEnum data_enum, void *val) {
    DataType data = {-1,-1};

    switch (data_enum) {
        case BOOL:
            data.data.boolean = val;
            break;
        case STRING:
            data.data.string = val;
            break;
        case UINT64_T:
            __uint64_t *tmp_UINT = val;
            data.data.uint64_val = *tmp_UINT;
            break;
        case FLOAT:
            float *tmp_FLOAT = val;
            data.data.float_val = *tmp_FLOAT;
            break;
        
        default:
            return data;
    }
    data.data_enum = data_enum;
    return data;
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

void *get_value(DataType data) {
    switch (data.data_enum) {
        case BOOL:
            return &(bool){data.data.boolean};
        case STRING:
            return &(char*){data.data.string};
        case UINT64_T:
            return &(__uint64_t){data.data.uint64_val};
        case FLOAT:
            return &(float){data.data.float_val};
        
        default:
            return NULL;
    }
}

void print(const char* str, DataType data) {
    // I don't understand this but oke
    __uint64_t i = 0;
    while (str[i] != '\0') i++;
    char *string = malloc(i*sizeof(str));
    strcpy(string, str);

    bool contains_percentage = (strstr(string, "%%") != NULL);

    if (contains_percentage) printf("%s", strtok(string, "%%"));
        switch (data.data_enum) {
            case BOOL:
                printf("%s", (data.data.boolean ? "true" : "false"));
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
    if (contains_percentage) printf("%s", strtok(NULL, "%%"));
}