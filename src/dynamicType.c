#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/dynamicType.h"

Data arr_get(DataArray array, __uint64_t index) {
    if (index > (array.size-1) || !array.size) {
        Data returnVal = {-1, -1};
        return returnVal;
    }
    return array.data[index];
}

void arr_put(DataArray *array, Data data) {
    if (!array->limit) array->limit++;

    __uint64_t byte_size = sizeof(Data)*array->size;
    Data *tmp = malloc(byte_size);
    memcpy(tmp, array->data, byte_size);

    if ((array->size+1) >= array->limit) {
        array->limit *= 2;
        //printf("%ld\n", array->limit);
        
        array->data = realloc(array->data, sizeof(Data)*array->limit);
        if (array->data == NULL) return;
    }

    memcpy(array->data, tmp, byte_size);
    free(tmp);
    array->data[array->size] = data;
    array->size++;
}

void arr_del(DataArray *array, __uint64_t index) {
    if (index > (array->size-1)) return;
    if (!(array->size-1)) {
        array->limit = 0;
        array->data = 0;
        free(array->data);
    }

    __uint64_t byte_size = sizeof(Data)*array->size;
    Data *tmp = malloc(byte_size);
    memcpy(tmp, array->data, byte_size);

    if ((array->size-1) <= (array->limit/2-1)) {
        array->limit /= 2;
        //printf("%ld\n", array->limit);

        array->data = realloc(array->data, sizeof(Data)*array->limit);
        if (array->data == NULL) return;
    }

    bool found;
    for (__uint64_t i = 0; i < array->size; i++) {
        if (i == index && !found) {
            found = true;
            continue;
        }
        array->data[i-found] = tmp[i];
    }
    free(tmp);
    array->size--;
}

Data data_init(DataType data_enum, void *val) {
    Data data = {-1,-1};

    switch (data_enum) {
        case BOOL:
            data.data.boolean = val;
            break;
        case STRING:
            data.data.string = val;
            break;
        case UINT64_T:
            data.data.uint64_val = *(uint64_t*){val};
            break;
        case FLOAT:
            data.data.float_val = *(float*){val};
            break;
        case ARRAY:
            data.data.array_val = val;
            break;
        
        default:
            return data;
    }
    data.data_enum = data_enum;
    return data;
}

char *data_typeof(Data data) {
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
        case ARRAY:
            t = malloc(9);
            strcpy(t, "ARRAY (");
            __uint64_t total = 0;
            for (__uint64_t j = 0; j < data.data.array_val->size; j++) {
                char *new = data_typeof(arr_get(*data.data.array_val,j));
                __uint64_t s = 0;
                while (new[s] != '\0') s++;
                if (j < data.data.array_val->size-1) s+=2;
                total += s;
                t = realloc(t, total+9);
                if (t == NULL) return "";
                strcat(t, new);
                if (j < data.data.array_val->size-1) strcat(t, ", ");
            }
            t[total+7] = ')';
            t[total+8] = '\0';
            break;
        
        default:
            t = "NULL";
            break;
    }
    return t;
}

void data_get(Data data, void *out) {
    switch (data.data_enum) {
        case BOOL:
            *(bool*)out = data.data.boolean;
            break;
        case STRING:
            *(char**)out = data.data.string;
            break;
        case UINT64_T:
            *(__uint64_t*)out = data.data.uint64_val;
            break;
        case FLOAT:
            *(float*)out = data.data.float_val;
            break;
        // can't do array
        
        default:
            break;
    }
}

void data_printf(const char* str, Data data) {

    char *startStr = malloc(1), *endStr = malloc(1);
    if (startStr == NULL || endStr == NULL) return;
    startStr[0] = '\0';
    endStr[0] = '\0';
    __uint64_t i = 0, splitIndex = 0;
    while (str[i] != '\0') {
        if (str[i] == '%') {
            splitIndex = ++i;
            continue;
        }

        if (splitIndex > 0) {
            endStr = realloc(endStr, i-splitIndex+1);
            if (endStr == NULL) return;
            endStr[i-splitIndex] = str[i];
            endStr[i-splitIndex+1] = '\0';
        } else {
            startStr = realloc(startStr, i+1);
            if (startStr == NULL) return;
            startStr[i] = str[i];
            startStr[i+1] = '\0';
        }
        i++;
    }

    printf("%s", startStr);
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
        case ARRAY:
            printf("(");
            for (__uint64_t j = 0; j < data.data.array_val->size; j++) {
                data_printf(j < data.data.array_val->size-1 ? "%%, " : "", arr_get(*data.data.array_val,j));
            }
            printf(")");
            break;
        
        default:
            printf("NULL");
            break;
    }
    printf("%s", endStr);
    free(startStr);
    free(endStr);
}