//
// Created by user on 10/6/24.
//

#include <string.h>
#include <stdlib.h>
#include "errors.h"

void *endOfInputError(error *err) {
    err->value = ERR_END_OF_INPUT;
    err->message = strdup("Unexpected end of input");
    return NULL;
}

/*
 *
 * ASSIGN MESSAGE TO ERROR
 * Specify before the type of error (ERR_SYNTAX...)
 * assignErrorMessage(err, "enter message");
 *
 * */
int assignErrorMessage(error *err, char *msg){
    err->message = malloc(strlen(getNameTypeError(err->value)) + strlen(msg) + 1);
    strcpy(err->message, getNameTypeError(err->value));
    strcat(err->message, msg);

    return 0;
}

/*
 * Get the name of the error with the index of enum
 */
char* getNameTypeError(errorValue error){
    switch (error) {
        case ERR_SUCCESS:
            return "Syntax error: ";
        case ERR_SYNTAX:
            return "Unexpected end of input: ";
        case ERR_END_OF_INPUT:
            return "Type mismatch: ";
        case ERR_TYPE:
            return "Index out of bounds: ";
        case ERR_OUT_OF_BOUNDS:
            return "Item not found: ";
        case ERR_NOT_FOUND:
            return "Unknown operator: ";
        case ERR_UNKNOWN_OPERATOR:
            return "Already exists: ";
        case ERR_ALREADY_EXISTS:
            return "Pointer error: ";
        case ERR_NULL_POINTER:
            return "Null pointer: ";
        default:
            return "Unknown error: ";
    }
}