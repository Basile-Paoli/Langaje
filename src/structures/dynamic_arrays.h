//
// Created by Basile on 02/10/2024.
//

#ifndef LANGAJE_DYNAMIC_ARRAYS_H
#define LANGAJE_DYNAMIC_ARRAYS_H


//Placeholder
typedef int token;

/**
 * This structure and the 3 following methods should be reimplemented with any appropriate type
 */
typedef struct {
    int len;
    int cap;
    token *values;
} dynamicTokenArray;

dynamicTokenArray *newDynamicTokenArray(int size);

void freeTokenArray(dynamicTokenArray *array);

dynamicTokenArray *appendToken(dynamicTokenArray *array, token value);

#endif //LANGAJE_DYNAMIC_ARRAYS_H
