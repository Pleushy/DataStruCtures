#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/dynamicType.h"

static void memfail() {
    fprintf(stderr, "Failed to allocate memory\n");
    return;
}

Data arr_get(DataArray array, uint64_t index) {
    if (index > (array.size-1) || !array.size) {
        Data returnVal = {-1, -1};
        return returnVal;
    }
    return array.data[index];
}

void arr_add(DataArray *array, Data data) {
    if (!array->limit) array->limit++;

    uint64_t byte_size = sizeof(Data)*array->size;
    Data *tmp = malloc(byte_size);
    if (tmp == NULL) {
        memfail();
        return;
    }
    memcpy(tmp, array->data, byte_size);

    if ((array->size+1) >= array->limit) {
        array->limit *= 2;
        //printf("%ld\n", array->limit);
        
        array->data = realloc(array->data, sizeof(Data)*array->limit);
        if (array->data == NULL) {
            memfail();
            return;
        }
    }

    memcpy(array->data, tmp, byte_size);
    free(tmp);
    tmp = NULL;
    array->data[array->size] = data;
    array->size++;
}

void arr_del(DataArray *array, uint64_t index) {
    if (!array->size) return;
    if (index > (array->size-1)) return;
    if (!(array->size-1)) {
        array->limit = 0;
        array->size = 0;
        array->data = 0;
        free(array->data);
        array->data = NULL;
        return;
    }

    uint64_t byte_size = sizeof(Data)*array->size;
    Data *tmp = malloc(byte_size);
    if (tmp == NULL) {
        memfail();
        return;
    }
    memcpy(tmp, array->data, byte_size);

    if ((array->size-1) <= (array->limit/2-1)) {
        array->limit /= 2;
        //printf("%ld\n", array->limit);

        array->data = realloc(array->data, sizeof(Data)*array->limit);
        if (array->data == NULL) {
            memfail();
            return;
        }
    }

    bool found = false;
    for (uint64_t i = 0; i < array->size; i++) {
        if (i == index && !found) {
            found = true;
            continue;
        }
        array->data[i-found] = tmp[i];
    }

    free(tmp);
    tmp = NULL;
    array->size--;
}

void arr_insert(DataArray *array, Data data, uint64_t index) {
    if (index>array->limit) return;
    if (!array->limit) array->limit++;

    uint64_t byte_size = sizeof(Data)*array->size;
    Data *tmp = malloc(byte_size);
    if (tmp == NULL) {
        memfail();
        return;
    }
    memcpy(tmp, array->data, byte_size);

    if ((array->size+1) >= array->limit) {
        array->limit *= 2;
        
        array->data = realloc(array->data, sizeof(Data)*array->limit);
        if (array->data == NULL) {
            memfail();
            return;
        }
    }

    bool found = false;
    for (uint64_t i = 0; i < array->size; i++) {
        if (i == index && !found) {
            found = true;
            array->data[i] = data;
        }
        array->data[i+found] = tmp[i];
    }

    free(tmp);
    tmp = NULL;

    array->size++;
}

uint64_t arr_is_empty(DataArray array) {
    return !array.size;
}

uint64_t arr_size(DataArray array) {
    return array.size;
}

Data *arr_get_values(DataArray array) {
    if (!array.size) return NULL;

    Data *data = malloc(array.size*sizeof(Data));
    if (data == NULL) {
        memfail();
        return NULL;
    }
    for (uint64_t i = 0; i < array.size; i++) {
        data[i] = array.data[i];
    }
    return data;
}

void arr_add_all(DataArray *array, Data *data) {
    uint64_t i = 0;
    while (data[i].data_enum) {
        arr_add(array, data[i]);
        i++;
    }
}

void arr_clear(DataArray *array) {
    if (!array->size) return;
    array->size = 0;
    array->limit = 0;
    free(array->data);
    array->data = NULL;
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
            t = malloc(2);
            if (t == NULL) {
                memfail();
                return NULL;
            }
            strcpy(t, "[");
            uint64_t total = 0;
            for (uint64_t j = 0; j < data.data.array_val->size; j++) {
                char *new = data_typeof(arr_get(*data.data.array_val,j));
                uint64_t s = 0;
                while (new[s] != '\0') s++;
                if (j < data.data.array_val->size-1) s+=2;
                total += s;
                t = realloc(t, total+2);
                if (t == NULL) {
                    memfail();
                    return NULL;
                }
                strcat(t, new);
                if (j < data.data.array_val->size-1) strcat(t, ", ");
            }
            t[total+1] = ']';
            t[total+2] = '\0';
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
            *(uint64_t*)out = data.data.uint64_val;
            break;
        case FLOAT:
            *(float*)out = data.data.float_val;
            break;
        case ARRAY: //TODO
            *(DataArray**)out = data.data.array_val;
            break;
        
        default:
            break;
    }
}

void data_printf(const char* str, Data data) {
    char *startStr = malloc(1), *endStr = malloc(1);
    if (startStr == NULL || endStr == NULL) {
        memfail();
        return;
    }
    uint64_t i = 0, splitIndex = 0;
    while (str[i] != '\0') {
        if (str[i] == '%') {
            splitIndex = ++i;
            continue;
        }

        if (splitIndex > 0) {
            endStr = realloc(endStr, i-splitIndex+1);
            if (endStr == NULL) {
                memfail();
                return;
            }
            endStr[i-splitIndex] = str[i];
            endStr[i-splitIndex+1] = '\0';
        } else {
            startStr = realloc(startStr, i+1);
            if (startStr == NULL) {
                memfail();
                return;
            }
            startStr[i] = str[i];
        }
        i++;
    }
    if (splitIndex) {
        startStr[i-(i-splitIndex)-2] = '\0';
    } else {
        startStr[i] = '\0';
    }
    endStr[i] = '\0';

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
            printf("[");
            for (uint64_t j = 0; j < data.data.array_val->size; j++) {
                data_printf(j < data.data.array_val->size-1 ? "%%, " : "", arr_get(*data.data.array_val,j));
            }
            printf("]");
            break;
        
        default:
            printf("NULL");
            break;
    }
    printf("%s", endStr);
    free(startStr);
    startStr = NULL;
    free(endStr);
    endStr = NULL;
}