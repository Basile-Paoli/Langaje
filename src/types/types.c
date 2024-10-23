//
// Created by Mael on 02/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "types.h"

/**
 * @brief Assigns a string to the `value._string` field of a `var` structure.
 * @warning Free the value you passed after calling the function !
 * @param v Pointer to a `var` structure with `type` set to `_string`.
 * @param value String to assign.
 * @return 0 on success, 1 on failure.
 */
int assignString(var *v, char *value) {
    int len = strlen(value);
    if (len < 0) {
        return 1;
    }
    if (v->value._string == NULL || len != strlen(v->value._string)) {
        v->value._string = (char *) realloc(v->value._string, sizeof(char) * (len + 1));
    }
    if (v->value._string == NULL) {
        return 1;
    }

    if (strcpy(v->value._string, value) == NULL) {
        return 1;
    }
    return 0;
}

/**
 * @brief Assigns a value to the `value._type` field of a `var` structure.
 * @warning Always pass a declared variable, never a direct value.
 * @param v Pointer to a `var` structure.
 * @param value Value to assign.
 * @return 0 on success, 1 on failure.
 */
int assign(var *v, void *value) {
    switch (v->type) {
        case _int:
            v->value._int = *(int *) value;
            break;
        case _float:
            v->value._float = *(float *) value;
            break;
        case _char:
            v->value._char = *(char *) value;
            break;
        case _string:
            assignString(v, (char *) value);
            break;
        default:
            return 1;
            break;
    }
    return 0;
}

/**
 * @brief Display the value based on the type
 * @param v Pointer to a `var` structure.
 */
void display(var *v) {
    switch (v->type) {
        case (_int):
            printf("%d\n", v->value._int);
            break;
        case (_float):
            printf("%f\n", v->value._float);
            break;
        case (_char):
            printf("%c\n", v->value._char);
            break;
        case (_array):
            for (int i = 0; i < v->value._array->length; i++) {
                display(&v->value._array->values[i]);
            }
            break;
        case (_string):
            printf("%s\n", v->value._string);
            break;
        default:
            printf("!Unknown type\n");
            break;
    }
}

var newArrayVar(int size, varType type) {
    array *arr = malloc(sizeof(array));
    arr->capacity = size == 0 ? 1 : size;
    arr->values = malloc(size * sizeof(var));
    arr->length = size;
    arr->type = type;

    var res;
    res.value._array = arr;
    res.type = _array;
    return res;
}

void appendToArrayVar(var *tab, var val) {
    assert(tab->type == _array);
    array *arr = tab->value._array;
    if (arr->length < arr->capacity) {
        arr->values[arr->length++] = val;
    } else {
        arr->capacity *= 2;
        arr->values = realloc(arr->values, arr->capacity);
        arr->values[arr->length++] = val;
    }
}
