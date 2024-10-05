//
// Created by Basile on 02/10/2024.
//

#include <stdlib.h>
#include <stdio.h>
#include "dynamic_arrays.h"


dynamicTokenArray *newDynamicTokenArray(int size) {
    dynamicTokenArray *array = malloc(sizeof(dynamicTokenArray));
    array->values = malloc(sizeof(token) * size);
    array->len = 0;
    array->cap = size;

    return array;
}

void freeTokenArray(dynamicTokenArray *array) {
    free(array->values);
    free(array);
}

dynamicTokenArray *appendToken(dynamicTokenArray *array, token value) {
    if (array->len < array->cap) {
        array->values[array->len++] = value;

        return array;
    }

    array->cap *= 2;
    token *newValues = realloc(array->values, array->cap * sizeof(token));
    free(array->values);
    array->values = newValues;
    array->values[array->len++] = value;

    return array;
}
